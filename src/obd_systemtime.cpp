/**
 * \author argawaen 
 * \date 08/06/2021
 */

#include <Arduino.h>
#include "obd_systemtime.h"
#include "obd_configfile.h"
#include "obd_system.h"
#include <sys/time.h>
#include "obd_filesystem.h"

namespace obd::time {

const String tsSave{"/dev/time"};

void clock::init() {
    if (getParent() != nullptr)
        fs = getParent()->getDriverAs<filesystem::driver>(F("FileSystem"));
    // restore time (not the true time but nearer than 1 jan 1970!)
    if (fs != nullptr) {
        if (fs->exists(tsSave)){
            auto f = fs->open(tsSave, F("r"));
            time_t ts = f.readString().toInt();
            f.close();
            timeval tv{ts,0};
            settimeofday(&tv,nullptr);

        }
    }
    configTime(timeZone.c_str(), poolServerName.c_str());
}

void clock::printInfo() {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(F(" ----- CLOCK INFORMATIONS -----"));
    getParentPrint()->print(F("Pool server       : "));
    getParentPrint()->println(poolServerName);
    getParentPrint()->print(F("Time Zone         : "));
    getParentPrint()->println(timeZone);
}

void clock::update(int64_t delta) {
    chrono += delta;
    if (chrono >= saveInterval ) {
        chrono = 0;
        // save current time so next boot will be loaded
        if (fs != nullptr) {
            auto f = fs->open(tsSave, F("w"));
            time_t a = getDate();
            f.write(a);
            f.close();
        }
    }
}

bool clock::treatCommand(const core::command& cmd) {
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

void clock::printHelp() {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(F("Help on system time"));
    getParentPrint()->println(F("date             print date and time of the system"));
    getParentPrint()->println(F("clockinfo        print time system infos"));
    getParentPrint()->println(F("clockpool <pool> change the name of the pool server"));
    getParentPrint()->println(F("clocktz   <tz>   change the time zone (see TZ.h for the format)"));
}

void clock::loadConfigFile() {
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

void clock::saveConfigFile() const {
    filesystem::configFile file(getParent());
    // parameter to save
    file.addConfigParameter("pool", poolServerName);
    file.addConfigParameter("tz", timeZone);
    //
    file.saveConfig(getName());
}

void clock::printDate() {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(getDateFormatted());
}

String clock::getDateFormatted() {
    time_t tv   = getDate();
    String tStr = ctime(&tv);
    tStr.replace("\n", "");
    tStr.replace("\r", "");
    return tStr;
}

time_t clock::getDate() const {
    timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec;
}

void clock::setPoolServer(const String& pool) {
    poolServerName = pool;
    init();
}

void clock::setTimeZone(const String& tz) {
    timeZone = tz;
    init();
}

}// namespace obd::time
