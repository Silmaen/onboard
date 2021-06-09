/**
 * \author argawaen 
 * \date 08/06/2021
 *
 */

#include "obd_systemtime.h"
#include "obd_network.h"
#include "obd_system.h"
#include <WiFiUdp.h>

namespace obd::time {

void clock::init() {
    if (getParent() != nullptr)
        net = getParent()->getDriverAs<network::driver>("Network");
    if (net != nullptr)
        udpConn = new WiFiUDP();
    if (udpConn != nullptr)
        udpConn->begin(port);
}

void clock::printInfo() {
}

void clock::update(int64_t delta) {
    timer += delta;
    if (timer == 0 || timer > updateInterval) {
        updateNTP();
        timer = 0;
    }
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
    String tStr = ctime(&currentEpoch);
    tStr.replace("\n", "");
    tStr.replace("\r", "");
    getParentPrint()->println(tStr);
}

void clock::sendNTPPacket() {
    uint8_t packetBuffer[NtpPacketSize];
    // set all bytes in the buffer to 0
    memset(static_cast<uint8_t*>(packetBuffer), 0, NtpPacketSize);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;// LI, Version, Mode
    packetBuffer[1] = 0;         // Stratum, or type of clock
    packetBuffer[2] = 6;         // Polling Interval
    packetBuffer[3] = 0xEC;      // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    udpConn->beginPacket(poolServerName.c_str(), 123);//NTP requests are to port 123
    udpConn->write(static_cast<uint8_t*>(packetBuffer), NtpPacketSize);
    udpConn->endPacket();
}

bool clock::updateNTP() {
    if (!checkNetworkState())
        return false;
    // send request
    sendNTPPacket();
    // wait for reply
    uint8_t timeout = 0;
    int cb          = 0;
    do {
        delay(10);
        cb = udpConn->parsePacket();
        if (timeout > 100) return false;// timeout after 1000ms
        ++timeout;
    } while (cb == 0);
    // parse reply
    uint8_t packetBuffer[NtpPacketSize];
    udpConn->read(static_cast<uint8_t*>(packetBuffer), NtpPacketSize);
    // seconds since Jan 1 1900
    currentEpoch = packetBuffer[40] << 24 | packetBuffer[41] << 16 | packetBuffer[42] << 8 | packetBuffer[43];
    // convert to seconds since Jan 1 1970;
    currentEpoch -= SevenZyYears;
    return true;
}
bool clock::checkNetworkState() {
    if (net == nullptr)
        return false;
    return net->getCurrentStatus() == network::Status::Connected || net->getCurrentStatus() == network::Status::ConnectedClient;
}

}// namespace obd::time
