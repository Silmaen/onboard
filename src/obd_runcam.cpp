/**
 * @author Silmaen
 * @date 01/07/2021.
 */

#include "obd_runcam.h"
#include <Arduino.h>

namespace obd::video {

constexpr uint8_t RC_HEADER        = 0xCC; ///< runCam protocol header
constexpr uint64_t ResponseTimeout = 1000; ///< timeout for message reception
constexpr uint64_t ConnexionCheckInterval = 5000; ///< interval between 2 checks for device

void RunCam::init() {
    uart.begin(115200);
    uart.clearWriteError();
    uart.flush();
    delay(10);
    getCameraInfo();
}

void RunCam::printInfo() {
    if (getParent() == nullptr)
        return;
    getParentPrint()->println(F(" ----- RUNCAM INFORMATION -----"));
    getParentPrint()->print("Debug print: ");
    printBool(debugPrint);
    if (isConnected) {
        getParentPrint()->print(F("RunCam Protocol version: ............... "));
        getParentPrint()->println(DeviceInfo.ProtocolVersion);
        getParentPrint()->print(F("RunCam Feature Simulate Power Button: .. "));
        printBool(DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_POWER_BUTTON);
        getParentPrint()->print(F("RunCam Feature Simulate wifi Button: ... "));
        printBool(DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_WIFI_BUTTON);
        getParentPrint()->print(F("RunCam Feature Change mode: ............ "));
        printBool(DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_CHANGE_MODE);
        getParentPrint()->print(F("RunCam Feature Simulate 5 key osb cable: "));
        printBool(DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_5_KEY_OSD_CABLE);
        getParentPrint()->print(F("RunCam Feature Device settings access: . "));
        printBool(DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_DEVICE_SETTINGS_ACCESS);
        getParentPrint()->print(F("RunCam Feature display port: ........... "));
        printBool(DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_DISPLAYP_PORT);
        getParentPrint()->print(F("RunCam Feature Start recording: ........ "));
        printBool(DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_START_RECORDING);
        getParentPrint()->print(F("RunCam Feature Stop recording: ......... "));
        printBool(DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_STOP_RECORDING);
        getParentPrint()->print(F("RunCam Feature FC attitude: ............ "));
        printBool(DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_FC_ATTITUDE);
    } else {
        getParentPrint()->println(F("Device not connected."));
    }
}

void RunCam::update(int64_t delta) {
    chrono += delta;
    if(!isConnected){
        if (chrono > ConnexionCheckInterval){
            chrono = 0;
            getCameraInfo();
        }
    }
}

bool RunCam::treatCommand(const core::command& cmd) {
    if (cmd.isCmd(F("runcamDebug"))) {
        debugPrint = !debugPrint;
        return true;
    }
    if (cmd.isCmd(F("runcamInfo"))) {
        printInfo();
        return true;
    }
    if (cmd.isCmd(F("runcamTest"))) {
        getCameraInfo();
        printInfo();
        return true;
    }
    return false;
}

void RunCam::printHelp() {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(F("Help for RunCam driver"));
    getParentPrint()->println(F("runcamDebug    toggle the debug printing into the multiStream"));
    getParentPrint()->println(F("runcamInfo     Print the info of the connected device"));
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
            getParentPrint()->println(F("RunCam GetInfo: bad response length."));
    }
    DeviceInfo.ProtocolVersion = response[0];
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_POWER_BUTTON = (response[2] & 1 << 0) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_WIFI_BUTTON = (response[2] & 1 << 1) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_CHANGE_MODE = (response[2] & 1 << 2) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_5_KEY_OSD_CABLE = (response[2] & 1 << 3) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_DEVICE_SETTINGS_ACCESS = (response[2] & 1 << 4) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_DISPLAYP_PORT = (response[2] & 1 << 5) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_START_RECORDING = (response[2] & 1 << 6) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_STOP_RECORDING = (response[2] & 1 << 7) != 0;
    DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_FC_ATTITUDE = (response[1] & 1 << 1) != 0;
}

std::vector<uint8_t> RunCam::sendCommand(RunCamCommand cmd, const std::vector<uint8_t>& params) {
    // only the get info command is allowed if not connected: it allow to determine if the device is connected
    if (!isConnected && (cmd != RunCamCommand::RCDEVICE_PROTOCOL_COMMAND_GET_DEVICE_INFO)) {
        getParentPrint()->println(F("RunCam sendCommand: no connexion for this command."));
        return std::vector<uint8_t>();
    }
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
    // effective message send
    uart.write(full_message.data(), full_message.size());
    full_message.clear();
    // wait for a response
    uint64_t start = millis();
    while (uart.available() == 0) {
        getParentPrint()->print(F("."));
        if (millis() - start > ResponseTimeout) {
            isConnected = false;
            getParentPrint()->print(F(" response timeout."));
            return std::vector<uint8_t>();
        }
        delay(10);// 10ms
    }
    getParentPrint()->println();
    // read the message
    resetCrc();
    bool msg = false;
    bool valid = false;
    while (uart.available() != 0) {
        uint8_t c = uart.read();
        if (debugPrint) {
            getParentPrint()->print(c, HEX);
            getParentPrint()->print(" / ");
            getParentPrint()->println(current_crc, HEX);
        }
        if (!msg) {
            if (c == RC_HEADER){
                msg = true;
            }else{
                delay(10);
                continue;
            }
            crc8_dvb_s2(c);
            continue;
        }
        if (c == current_crc) {
            valid = true;
            break;
        }
        crc8_dvb_s2(c);
        full_message.push_back(c);
    }
    // active connexion
    if (cmd == RunCamCommand::RCDEVICE_PROTOCOL_COMMAND_GET_DEVICE_INFO)
        isConnected = true;
    // message verification
    if (!valid)
        return std::vector<uint8_t>();
    if (debugPrint) {
        getParentPrint()->print(F("message: "));
        for (auto c : full_message) {
            getParentPrint()->print(c, HEX);
            getParentPrint()->print(" ");
        }
        getParentPrint()->println();
    }
    // return message content
    return full_message;
}

void RunCam::printBool(bool ptr) {
    if (getParentPrint() == nullptr)
        return;
    if (ptr) {
        getParentPrint()->println(F("true"));
    }else{
        getParentPrint()->println(F("false"));
    }
}

}// namespace obd::video
