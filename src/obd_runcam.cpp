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
    if (cmd.isCmd(F("runcamCmd"))) {
        parseCmd(cmd.getParams());
        return true;
    }
    if (cmd.isCmd(F("runcam5key"))) {
        parse5Key(cmd.getParams());
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
    getParentPrint()->println(F("runcamCmd      Send a command to the connected device. Valid command are:"));
    getParentPrint()->println(F("                wifi   Simulate the wifi button"));
    getParentPrint()->println(F("                power  Simulate the power button"));
    getParentPrint()->println(F("                mode   Change camera mode"));
    getParentPrint()->println(F("                start  Start Camera"));
    getParentPrint()->println(F("                stop   Stop Camera"));
    getParentPrint()->println(F("runcam5key     Send a 5 key action to the connected device. Valid parameters are:"));
    getParentPrint()->println(F("                set    Simulate the push on button"));
    getParentPrint()->println(F("                left   Simulate the left direction"));
    getParentPrint()->println(F("                right  Simulate the right direction"));
    getParentPrint()->println(F("                up     Simulate the up direction"));
    getParentPrint()->println(F("                down   Simulate the down direction"));
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

std::vector<uint8_t> RunCam::sendCommand(RunCamCommand cmd, const std::vector<uint8_t>& params, bool expectResponse) {
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
    if (!expectResponse)
        return std::vector<uint8_t>();
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

void RunCam::CameraControl(const RunCamControlCommand& command) {
    if (!isConnected)
        return;
    if (command == RunCamControlCommand::RCDEVICE_PROTOCOL_SIMULATE_WIFI_BTN && !DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_WIFI_BUTTON ){
        if (getParentPrint() != nullptr)
            getParentPrint()->println(F("RunCam CameraControl: Unable to simulate Wifi Button, no feature."));
        return;
    }
    if (command == RunCamControlCommand::RCDEVICE_PROTOCOL_SIMULATE_POWER_BTN && !DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_POWER_BUTTON ){
        if (getParentPrint() != nullptr)
            getParentPrint()->println(F("RunCam CameraControl: Unable to simulate power Button, no feature."));
        return;
    }
    if (command == RunCamControlCommand::RCDEVICE_PROTOCOL_CHANGE_MODE && !DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_CHANGE_MODE ){
        if (getParentPrint() != nullptr)
            getParentPrint()->println(F("RunCam CameraControl: Unable to change mode, no feature."));
        return;
    }
    if (command == RunCamControlCommand::RCDEVICE_PROTOCOL_CHANGE_START_RECORDING && !DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_START_RECORDING ){
        if (getParentPrint() != nullptr)
            getParentPrint()->println(F("RunCam CameraControl: Unable to start camera, no feature."));
        return;
    }
    if (command == RunCamControlCommand::RCDEVICE_PROTOCOL_CHANGE_START_RECORDING && !DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_STOP_RECORDING ){
        if (getParentPrint() != nullptr)
            getParentPrint()->println(F("RunCam CameraControl: Unable to stop camera, no feature."));
        return;
    }

    std::vector<uint8_t> response = sendCommand(RunCamCommand::RCDEVICE_PROTOCOL_COMMAND_CAMERA_CONTROL, std::vector<uint8_t>{static_cast<uint8_t>(command)}, false);
    if (!response.empty()){
        if (getParentPrint() != nullptr)
            getParentPrint()->println(F("RunCam CameraControl: bad response length."));
    }

}

void RunCam::parseCmd(const String& cmd){
    if (cmd == F("wifi")) {
        CameraControl(RunCamControlCommand::RCDEVICE_PROTOCOL_SIMULATE_WIFI_BTN);
    }else if (cmd == F("power")) {
        CameraControl(RunCamControlCommand::RCDEVICE_PROTOCOL_SIMULATE_POWER_BTN);
    }else if (cmd == F("mode")) {
        CameraControl(RunCamControlCommand::RCDEVICE_PROTOCOL_CHANGE_MODE);
    }else if (cmd == F("start")) {
        CameraControl(RunCamControlCommand::RCDEVICE_PROTOCOL_CHANGE_START_RECORDING);
    }else if (cmd == F("stop")) {
        CameraControl(RunCamControlCommand::RCDEVICE_PROTOCOL_CHANGE_START_RECORDING);
    }
}

void RunCam::simulate5keyRemoteControl(const RunCam::RunCam5keyControl& command) {
    if (!isConnected)
        return;
    if (!DeviceInfo.RCDEVICE_PROTOCOL_FEATURE_SIMULATE_5_KEY_OSD_CABLE)
        return;
    std::vector<uint8_t> response = sendCommand(RunCamCommand::RCDEVICE_PROTOCOL_COMMAND_CAMERA_CONTROL, std::vector<uint8_t>{static_cast<uint8_t>(command)});
    if (!response.empty()){
        if (getParentPrint() != nullptr)
            getParentPrint()->println(F("RunCam parseCmd: bad response length."));
    }
}

void RunCam::parse5Key(const String& cmd){
    if (cmd == F("set")) {
        simulate5keyRemoteControl(RunCam5keyControl::RCDEVICE_PROTOCOL_5KEY_SIMULATION_SET);
    }else if (cmd == F("left")) {
        simulate5keyRemoteControl(RunCam5keyControl::RCDEVICE_PROTOCOL_5KEY_SIMULATION_LEFT);
    }else if (cmd == F("right")) {
        simulate5keyRemoteControl(RunCam5keyControl::RCDEVICE_PROTOCOL_5KEY_SIMULATION_RIGHT);
    }else if (cmd == F("up")) {
        simulate5keyRemoteControl(RunCam5keyControl::RCDEVICE_PROTOCOL_5KEY_SIMULATION_UP);
    }else if (cmd == F("down")) {
        simulate5keyRemoteControl(RunCam5keyControl::RCDEVICE_PROTOCOL_5KEY_SIMULATION_DOWN);
    }
}


}// namespace obd::video
