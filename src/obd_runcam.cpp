/**
 * @author Silmaen
 * @date 01/07/2021.
 */

#include "obd_runcam.h"
#include <Arduino.h>

namespace obd::video {

constexpr uint8_t RC_HEADER        = 0xCC;///< runCam protocol header
constexpr uint64_t ResponseTimeout = 100; ///< timeout for message reception


void RunCam::init() {
    getCameraInfo();
}

void RunCam::printInfo() {
    if (getParent() == nullptr)
        return;
    getParentPrint()->println(F(" ----- RUNCAM INFORMATION -----"));
    if (isConnected) {
    } else {
        getParentPrint()->println(F("Device not connected."));
    }
}

bool RunCam::treatCommand(const core::command& cmd) {
    if (cmd.isCmd(F "runcamInfo")) {
        printInfo();
    }
}

void RunCam::printHelp() {
}

void RunCam::loadConfigFile() {
}

void RunCam::saveConfigFile() const {
}

void RunCam::getCameraInfo() {
    std::vector<uint8_t> response = sendCommand(RunCamCommand::RCDEVICE_PROTOCOL_COMMAND_GET_DEVICE_INFO, std::vector<uint8_t>());
    if (!isConnected)
        return;
    if (response.size() != 3){
        if (getParentPrint() != nullptr)
            getParentPrint()->println("RunCam GetInfo: bad response length.");
    }
    DeviceInfo.ProtocolVersion = response[0];
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_POWER_BUTTON = (response[1] & 1 << 0) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_WIFI_BUTTON = (response[1] & 1 << 1) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_CHANGE_MODE = (response[1] & 1 << 2) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_5_KEY_OSD_CABLE = (response[1] & 1 << 3) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_DEVICE_SETTINGS_ACCESS = (response[1] & 1 << 4) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_DISPLAYP_PORT = (response[1] & 1 << 5) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_START_RECORDING = (response[1] & 1 << 6) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_STOP_RECORDING = (response[1] & 1 << 7) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_FC_ATTITUDE = (response[2] & 1 << 1) != 0;
}

std::vector<uint8_t> RunCam::sendCommand(RunCamCommand cmd, const std::vector<uint8_t>& params) {
    // only the get info command is allowed if not connected: it allow to determine if the device is connected
    if (!isConnected && (cmd != RunCamCommand::RCDEVICE_PROTOCOL_COMMAND_GET_DEVICE_INFO))
        return std::vector<uint8_t>();
    // creation of the message to send
    std::vector<uint8_t> full_message;
    resetCrc();
    full_message.push_back(RC_HEADER);
    crc8_dvb_s2(RC_HEADER);
    full_message.push_back(static_cast<uint8_t>(cmd));
    crc8_dvb_s2(static_cast<uint8_t>(cmd));
    for (auto p : params) {
        full_message.push_back(p);
        crc8_dvb_s2(p);
    }
    full_message.push_back(current_crc);
    crc8_dvb_s2(current_crc);
    // effective message send
    Serial1.write(full_message.data(), full_message.size());
    full_message.clear();
    // wait for a response
    uint64_t start = millis();
    while (!Serial1.available()) {
        if (millis() - start < ResponseTimeout) {
            isConnected = false;
            return std::vector<uint8_t>();
        }
        delay(10);// 10ms
    }
    // read the message
    while (Serial1.available()) {
        full_message.push_back(Serial1.read());
    }
    // active connexion
    if (cmd == RunCamCommand::RCDEVICE_PROTOCOL_COMMAND_GET_DEVICE_INFO)
        isConnected = true;
    // message verification
    if (full_message[0] != RC_HEADER) {
        if (getParentPrint() != nullptr)
            getParentPrint()->println("RunCam receive bad header");
        return std::vector<uint8_t>();
    }
    resetCrc();
    for (size_t i = 0; i < full_message.size() - 1; ++i) {
        crc8_dvb_s2(full_message[i]);
    }
    if (current_crc != full_message.back()) {
        if (getParentPrint() != nullptr)
            getParentPrint()->println("RunCam receive bad crc");
        return std::vector<uint8_t>();
    }
    // return message content
    return std::vector<uint8_t>(full_message.begin() + 1, full_message.end() - 1);
}


}// namespace obd::video
