/**
 * @file Clock.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "Clock.h"
#include "data/DataUtils.h"
#include "fs/ConfigFile.h"
#include "native/fakeArduino.h"
#include <sys/time.h>

namespace obd::time {


void Clock::init() {
    Node::init();
    if (!initialized())
        return;
    if (!checkFs() )
        return;
    loadConfig();
    configTime();
}

bool Clock::checkFs() const{
    if (!fileSystem)
        return false;
    // restore time (not the true time but nearer than 1 jan 1970!)
    if (!fileSystem->initialized() )
        return false;
    return true;
}

void Clock::configTime() {
#ifdef ARDUINO
    if (fileSystem->exists(fs::Path(OString(config::tsSave)))) {
        fs::TextFile timeFile(fileSystem, fs::Path(config::tsSave), fs::ios::in);
        time_t ts = atoi(timeFile.readLine().c_str());
        timeFile.close();
        timeval tv{ts, 0};
        settimeofday(&tv, nullptr);
    }
    ::configTime(_timeZone.c_str(), poolServerName.c_str());
#endif
}

OString Clock::info()const {
    OString result;
    result = "----- CLOCK INFORMATION -----\n" ;
    result += OString("Pool server       : ") + poolServerName + "\n";
    result += OString("Time Zone         : ") + _timeZone + "\n";
    return result;
}

void Clock::preTreatment() {
    uint64_t timeStamp = micros();
    int64_t delta = static_cast<int64_t>(timeStamp) - static_cast<int64_t>(timestamp);
    timestamp    = timeStamp;
    chronometer += delta;
    if (chronometer >= config::saveInterval) {
        chronometer = 0;
        // save current time so next boot will be loaded
        if (checkFs()){
            fs::TextFile timeFile(fileSystem, fs::Path(config::tsSave), fs::ios::out);
            time_t date = getDate();
            timeFile.write(OString(date));
            timeFile.close();
        }
    }
}

bool Clock::treatMessage(const Message& message) {
    if (Node::treatMessage(message)){
        return true;
    }
    if (message.getType() == Message::MessageType::Command) {
        if (message.getBaseCommand() == F("date")) {
            broadcastMessage(message.getSource(), getDateFormatted(), MessageType::Reply);
            return true;
        }
        if (message.getBaseCommand() == F("pool")) {
            if (message.hasParams()) {
                setPoolServer(message.getParams()[0]);
            }else{
                broadcastMessage(message.getSource(), "pool: need a parameter", MessageType::Error);
            }
            return true;
        }
        if (message.getBaseCommand() == F("zone")) {
            if (message.hasParams()) {
                setTimeZone(message.getParams()[0]);
            }else{
                broadcastMessage(message.getSource(), "zone: need a parameter", MessageType::Error);
            }
            return true;
        }
    }
    return false;
}

bool Clock::pushCommand(const Message& message) {
    if (Node::pushCommand(message))
        return true;
    if (message.getBaseCommand() == F("date")) {
        getMessages().push(message);
        return true;
    }
    if (message.getBaseCommand() == F("pool")) {
        getMessages().push(message);
        return true;
    }
    if (message.getBaseCommand() == F("zone")) {
        getMessages().push(message);
        return true;
    }
    return false;
}

void Clock::loadConfig() {
    if (!checkFs() )
        return;
    fs::ConfigFile configFile(fileSystem);
    configFile.loadConfig(name());
    // parameters to load:
    if (configFile.hasKey("pool")) {
        poolServerName = configFile.getKey("pool");
    }
    if (configFile.hasKey("tz")) {
        _timeZone = configFile.getKey("tz");
    }
}

void Clock::saveConfig() const {
    if (!checkFs() )
        return;
    fs::ConfigFile configFile(fileSystem);
    // parameter to save
    configFile.addConfigParameter("pool", poolServerName);
    configFile.addConfigParameter("tz", _timeZone);
    //
    configFile.saveConfig(name());
}

OString Clock::getDateFormatted() {
    return formatTime(getDate());
}

time_t Clock::getDate() {
    timeval tval{};
    gettimeofday(&tval, nullptr);
    return tval.tv_sec;
}

void Clock::setPoolServer(const OString& pool) {
    poolServerName = pool;
    configTime();
}

void Clock::setTimeZone(const OString& timeZone) {
    _timeZone = timeZone;
    configTime();
}

OString Clock::formatTime(const time_t& time) {
    OString tStr = ctime(&time);
    tStr = data::ReplaceAll(tStr, "\n", "");
    tStr = data::ReplaceAll(tStr, "\r", "");
    return tStr;
}

void Clock::accelerateTime(uint64_t addedTime) {
    chronometer += addedTime;
}

}// namespace obd::time
