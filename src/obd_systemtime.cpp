/**
 * \author argawaen 
 * \date 08/06/2021
 *
 */

#include "obd_systemtime.h"
#include "obd_network.h"
#include "obd_system.h"
#include <ctime>

namespace obd::time {

void clock::init() {
    configTime(TZ_Europe_Paris, poolServerName.c_str());
}

void clock::printInfo() {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(F("System Clock informations"));
    getParentPrint()->print(F("Max Number of servers: "));
    getParentPrint()->println(SNTP_MAX_SERVERS);
}

void clock::update(int64_t delta) {
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
    return false;
}

void clock::printHelp() {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(F("Help on system time"));
    getParentPrint()->println(F("date      print date and time of the system"));
}

void clock::loadConfigFile() {}

void clock::saveConfigFile() const {}

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

}// namespace obd::time
