/**
 * \author argawaen 
 * \date 08/06/2021
 *
 */

#include "obd_systemtime.h"
#include "obd_network.h"
#include "obd_system.h"

namespace obd::time {

void clock::init() {
    net = getParent()->getDriverAs<network::driver>("Network");
    EspClass::rtcUserMemoryRead(0, reinterpret_cast<uint32_t*>(&epoch), sizeof(epoch));
}

void clock::printInfo() {
}

void clock::update(int64_t  delta) {
    lastTs += delta;
    if (net->getCurrentStatus() == network::Status::ConnectedClient ||
        net->getCurrentStatus() == network::Status::Connected) {
        epoch = net->getNtpClient().getEpochTime();
    }else{
        epoch += static_cast<time_t>(lastTs/1000);
    }
    EspClass::rtcUserMemoryWrite(0, reinterpret_cast<uint32_t*>(&epoch), sizeof(epoch));
    if (lastTs>1000)
        lastTs = 0;
}

bool clock::treatCommand(const core::command& cmd) {
    if (cmd.isCmd(F("date"))) {
        printDate();
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
    String tstr = ctime(&epoch);
    tstr.replace("\n", "");
    tstr.replace("\r", "");
    getParentPrint()->println(tstr);
}

}// namespace obd::time
