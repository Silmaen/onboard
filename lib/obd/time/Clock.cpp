/**
 * @file Clock.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "Clock.h"
#include "core/System.h"
#include "data/DataUtils.h"
#include "fs/ConfigFile.h"
#include "native/fakeArduino.h"
#include <sys/time.h>

namespace obd::time {


bool Clock::init() {
    if (getParent() == nullptr)
        return false;
    fileSystem = getParent()->getDriver<fs::FileSystem>();
    // restore time (not the true time but nearer than 1 jan 1970!)
    if (!fileSystem->initialized() )
        return core::BaseDriver::init();
    loadConfigFile();
    configTime();
    return core::BaseDriver::init();
}

void Clock::configTime() {
#ifdef ARDUINO
    if (fileSystem->exists(fs::Path(std::string(config::tsSave)))) {
        fs::TextFile timeFile(fileSystem.get(), fs::Path(config::tsSave), fs::ios::in);
        time_t ts = atoi(timeFile.readLine().c_str());
        timeFile.close();
        timeval tv{ts, 0};
        settimeofday(&tv, nullptr);
    }
    configTime(timeZone.c_str(), poolServerName.c_str());
#endif
}

void Clock::printInfo() {
    println(F(" ----- CLOCK INFORMATION -----"));
    print(F("Pool server       : "));
    println(poolServerName);
    print(F("Time Zone         : "));
    println(_timeZone);
}

void Clock::update(int64_t delta) {
    chronometer += delta;
    if (chronometer >= config::saveInterval) {
        chronometer = 0;
        // save current time so next boot will be loaded
        if (fileSystem != nullptr) {
            fs::TextFile timeFile(fileSystem, fs::Path(config::tsSave), fs::ios::out);
            time_t date = getDate();
            std::stringstream oss;
            oss << date;
            timeFile.write(oss.str());
            timeFile.close();
        }
    }
}

bool Clock::treatCommand(const core::Command& cmd) {
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
    fs::ConfigFile configFile(fileSystem);
    configFile.loadConfig(getName());
    // parameters to load:
    if (configFile.hasKey("pool")) {
        poolServerName = configFile.getKey("pool");
    }
    if (configFile.hasKey("tz")) {
        _timeZone = configFile.getKey("tz");
    }
}

void Clock::saveConfigFile() const {
    fs::ConfigFile configFile(fileSystem);
    // parameter to save
    configFile.addConfigParameter("pool", poolServerName);
    configFile.addConfigParameter("tz", _timeZone);
    //
    configFile.saveConfig(getName());
}

void Clock::printDate() {
    println(getDateFormatted());
}

std::string Clock::getDateFormatted() {
    return formatTime(getDate());
}

time_t Clock::getDate() {
    timeval tval{};
    gettimeofday(&tval, nullptr);
    return tval.tv_sec;
}

void Clock::setPoolServer(const std::string& pool) {
    poolServerName = pool;
    configTime();
}

void Clock::setTimeZone(const std::string& timeZone) {
    _timeZone = timeZone;
    configTime();
}

std::string Clock::formatTime(const time_t& time) {
    std::string tStr = ctime(&time);
    data::ReplaceAll(tStr, "\n", "");
    data::ReplaceAll(tStr, "\r", "");
    return tStr;
}

}// namespace obd::time
