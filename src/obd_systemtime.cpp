/**
 * @author Silmaen
 * @date 08/06/2021
 */

#include <Arduino.h>
#include "obd_systemtime.h"
#include "obd_configfile.h"
#include "obd_system.h"
#include <sys/time.h> // NOLINT(hicpp-deprecated-headers,modernize-deprecated-headers)
#include "obd_filesystem.h"

namespace obd::time {

void Clock::init() {
    fs = getDriverAs<file::FileSystem>(F("FileSystem"));
    // restore time (not the true time but nearer than 1 jan 1970!)
    if (fs != nullptr) {
        if (fs->exists(config::tsSave)){
            auto f = fs->open(config::tsSave, F("r"));
            time_t ts = f.readString().toInt();
            f.close();
            timeval tv{ts,0};
            settimeofday(&tv,nullptr);

        }
    }
    configTime(timeZone.c_str(), poolServerName.c_str());
}

void Clock::printInfo() {
    println(F(" ----- CLOCK INFORMATION -----"));
    print(F("Pool server       : "));
    println(poolServerName);
    print(F("Time Zone         : "));
    println(timeZone);
}

void Clock::update(int64_t delta) {
    chronometer += delta;
    if (chronometer >= config::saveInterval ) {
        chronometer = 0;
        // save current time so next boot will be loaded
        if (fs != nullptr) {
            auto f = fs->open(config::tsSave, F("w"));
            time_t a = getDate();
            f.write(a);
            f.close();
        }
    }
}

bool Clock::treatCommand(const core::command& cmd) {
    if (cmd.isCmd(F("date"))) {
        printDate();
        return true;
    }
    if (cmd.isCmd(F("clockinfo"))) {
        printInfo();
        return true;
    }
    if (cmd.isCmd(F("clockpool"))) {
        setPoolServer(cmd.getParams());
        return true;
    }
    if (cmd.isCmd(F("clocktz"))) {
        setTimeZone(cmd.getParams());
        return true;
    }
    return false;
}

void Clock::printHelp() {
    println(F("Help on system time"));
    println(F("date             print date and time of the system"));
    println(F("clockinfo        print time system infos"));
    println(F("clockpool <pool> change the name of the pool server"));
    println(F("clocktz   <tz>   change the time zone (see TZ.h for the format)"));
}

void Clock::loadConfigFile() {
    filesystem::configFile file(getParent());
    file.loadConfig(getName());
    // parameters to load:
    if (file.hasKey("pool")){
        poolServerName = file.getKey("pool");
    }
    if (file.hasKey("tz")){
        timeZone = file.getKey("tz");
    }
}

void Clock::saveConfigFile() const {
    filesystem::configFile file(getParent());
    // parameter to save
    file.addConfigParameter("pool", poolServerName);
    file.addConfigParameter("tz", timeZone);
    //
    file.saveConfig(getName());
}

void Clock::printDate() {
    println(getDateFormatted());
}

String Clock::getDateFormatted() {
    return formatTime(getDate());
}

time_t Clock::getDate() {
    timeval tv{};
    gettimeofday(&tv, nullptr);
    return tv.tv_sec;
}

void Clock::setPoolServer(const String& pool) {
    poolServerName = pool;
    init();
}

void Clock::setTimeZone(const String& tz) {
    timeZone = tz;
    init();
}

String Clock::formatTime(const time_t& time) {
    String tStr = ctime(&time);
    tStr.replace("\n", "");
    tStr.replace("\r", "");
    return tStr;
}

}// namespace obd::time
