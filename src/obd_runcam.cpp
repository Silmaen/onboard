/**
 * @author Silmaen
 * @date 01/07/2021.
 */

#include "obd_runcam.h"
#include <Arduino.h>

namespace obd::video {

constexpr uint8_t RC_HEADER               = 0xCC;///< runCam protocol header
constexpr uint64_t ResponseTimeout        = 1000;///< timeout for message reception
constexpr uint64_t ConnexionCheckInterval = 5000;///< interval between 2 checks for device

void RunCam::init() {
    uart.begin(115200);
    uart.clearWriteError();
    uart.flush();
    delay(10);
    getCameraInfo();
}

void RunCam::printInfo() {
    println(F(" ----- RUNCAM INFORMATION -----"));
    print(F("Debug print: "));
    printBool(debugPrint);
    if (isConnected) {
        print(F("RunCam 5Key pad connected: ............. "));
        printBool(is5keyConnected);
        print(F("RunCam Protocol version: ............... "));
        println(DeviceInfo.ProtocolVersion);
        print(F("RunCam Feature Simulate Power Button: .. "));
        printBool(DeviceInfo.hasFeature(Feature::SIMULATE_POWER_BUTTON));
        print(F("RunCam Feature Simulate wifi Button: ... "));
        printBool(DeviceInfo.hasFeature(Feature::SIMULATE_WIFI_BUTTON));
        print(F("RunCam Feature Change mode: ............ "));
        printBool(DeviceInfo.hasFeature(Feature::CHANGE_MODE));
        print(F("RunCam Feature Simulate 5 key osb cable: "));
        printBool(DeviceInfo.hasFeature(Feature::SIMULATE_5_KEY_OSD_CABLE));
        print(F("RunCam Feature Device settings access: . "));
        printBool(DeviceInfo.hasFeature(Feature::DEVICE_SETTINGS_ACCESS));
        print(F("RunCam Feature display port: ........... "));
        printBool(DeviceInfo.hasFeature(Feature::DISPLAYP_PORT));
        print(F("RunCam Feature Start recording: ........ "));
        printBool(DeviceInfo.hasFeature(Feature::START_RECORDING));
        print(F("RunCam Feature Stop recording: ......... "));
        printBool(DeviceInfo.hasFeature(Feature::STOP_RECORDING));
        print(F("RunCam Feature FC attitude: ............ "));
        printBool(DeviceInfo.hasFeature(Feature::FC_ATTITUDE));
    } else {
        println(F("Device not connected."));
    }
}

void RunCam::update(int64_t delta) {
    chrono += delta;
    if (!isConnected) {
        if (chrono > ConnexionCheckInterval) {
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
    println(F("Help for RunCam driver"));
    println(F("runcamDebug    toggle the debug printing into the multiStream"));
    println(F("runcamInfo     Print the info of the connected device"));
    println(F("runcamCmd      Send a command to the connected device. Valid command are:"));
    println(F("                wifi   Simulate the wifi button"));
    println(F("                power  Simulate the power button"));
    println(F("                mode   Change camera mode"));
    println(F("                start  Start Camera"));
    println(F("                stop   Stop Camera"));
    println(F("runcam5key     Send a 5 key action to the connected device. Valid parameters are:"));
    println(F("                open    Begin connexion to the 5key pad"));
    println(F("                close   Close connexion to the 5key pad"));
    println(F("                set     Simulate the push on button"));
    println(F("                left    Simulate the left direction"));
    println(F("                right   Simulate the right direction"));
    println(F("                up      Simulate the up direction"));
    println(F("                down    Simulate the down direction"));
    println(F("                release Simulate the down direction"));
}

void RunCam::loadConfigFile() {
}

void RunCam::saveConfigFile() const {
}

void RunCam::getCameraInfo() {
    std::vector<uint8_t> response = sendCommand(Command::GET_DEVICE_INFO, std::vector<uint8_t>());
    if (!isConnected)
        return;
    if (response.size() != 3) {
        if (getParentPrint() != nullptr)
            println(F("RunCam GetInfo: bad response length."));
    }
    DeviceInfo.ProtocolVersion = response[0];
    DeviceInfo.Features        = (response[1] << 8U) | response[2];
}

std::vector<uint8_t> RunCam::sendCommand(Command cmd, const std::vector<uint8_t>& params, bool expectResponse) {
    // only the get info command is allowed if not connected: it allow to determine if the device is connected
    if (!isConnected && (cmd != Command::GET_DEVICE_INFO)) {
        println(F("RunCam sendCommand: no connexion for this command."));
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
        print(F("."));
        if (millis() - start > ResponseTimeout) {
            isConnected = false;
            print(F(" response timeout."));
            return std::vector<uint8_t>();
        }
        delay(10);// 10ms
    }
    println();
    // read the message
    resetCrc();
    bool msg   = false;
    bool valid = false;
    while (uart.available() != 0) {
        uint8_t c = uart.read();
        if (debugPrint) {
            print(c, HEX);
            print(" / ");
            println(current_crc, HEX);
        }
        if (!msg) {
            if (c == RC_HEADER) {
                msg = true;
            } else {
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
    if (cmd == Command::GET_DEVICE_INFO)
        isConnected = true;
    // message verification
    if (!valid)
        return std::vector<uint8_t>();
    if (debugPrint) {
        print(F("message: "));
        for (auto c : full_message) {
            print(c, HEX);
            print(" ");
        }
        println();
    }
    // return message content
    return full_message;
}

void RunCam::printBool(bool ptr) {
    if (getParentPrint() == nullptr)
        return;
    if (ptr) {
        println(F("true"));
    } else {
        println(F("false"));
    }
}

void RunCam::CameraControl(const ControlCommand& command) {
    if (!isConnected)
        return;
    if (command == ControlCommand::SIMULATE_WIFI_BTN && !DeviceInfo.hasFeature(Feature::SIMULATE_WIFI_BUTTON)) {
        if (getParentPrint() != nullptr)
            println(F("RunCam CameraControl: Unable to simulate Wifi Button, no feature."));
        return;
    }
    if (command == ControlCommand::SIMULATE_POWER_BTN && !DeviceInfo.hasFeature(Feature::SIMULATE_POWER_BUTTON)) {
        if (getParentPrint() != nullptr)
            println(F("RunCam CameraControl: Unable to simulate power Button, no feature."));
        return;
    }
    if (command == ControlCommand::CHANGE_MODE && !DeviceInfo.hasFeature(Feature::CHANGE_MODE)) {
        if (getParentPrint() != nullptr)
            println(F("RunCam CameraControl: Unable to change mode, no feature."));
        return;
    }
    if (command == ControlCommand::CHANGE_START_RECORDING && !DeviceInfo.hasFeature(Feature::START_RECORDING)) {
        if (getParentPrint() != nullptr)
            println(F("RunCam CameraControl: Unable to start camera, no feature."));
        return;
    }
    if (command == ControlCommand::CHANGE_START_RECORDING && !DeviceInfo.hasFeature(Feature::STOP_RECORDING)) {
        if (getParentPrint() != nullptr)
            println(F("RunCam CameraControl: Unable to stop camera, no feature."));
        return;
    }

    std::vector<uint8_t> response = sendCommand(Command::CAMERA_CONTROL, std::vector<uint8_t>{static_cast<uint8_t>(command)}, false);
    if (!response.empty()) {
        if (getParentPrint() != nullptr)
            println(F("RunCam CameraControl: bad response length."));
    }
}

void RunCam::parseCmd(const String& cmd) {
    if (cmd == F("wifi")) {
        CameraControl(ControlCommand::SIMULATE_WIFI_BTN);
    } else if (cmd == F("power")) {
        CameraControl(ControlCommand::SIMULATE_POWER_BTN);
    } else if (cmd == F("mode")) {
        CameraControl(ControlCommand::CHANGE_MODE);
    } else if (cmd == F("start")) {
        CameraControl(ControlCommand::CHANGE_START_RECORDING);
    } else if (cmd == F("stop")) {
        CameraControl(ControlCommand::CHANGE_STOP_RECORDING);
    }
}

void RunCam::simulate5keyRemoteControl(const RunCam::key5Control& command) {
    if (!isConnected)
        return;
    if (!DeviceInfo.hasFeature(Feature::SIMULATE_5_KEY_OSD_CABLE))
        return;
    std::vector<uint8_t> response;
    if (command == key5Control::SIMULATION_RELEASE) {
        response = sendCommand(Command::KEY5_SIMULATION_RELEASE, std::vector<uint8_t>{});
    } else {
        if (is5keyConnected)
            response = sendCommand(Command::KEY5_SIMULATION_PRESS, std::vector<uint8_t>{static_cast<uint8_t>(command)});
    }
    if (!response.empty()) {
        if (getParentPrint() != nullptr)
            println(F("RunCam parseCmd: bad response length."));
    }
}

void RunCam::parse5Key(const String& cmd) {
    if (cmd == F("set")) {
        simulate5keyRemoteControl(key5Control::SIMULATION_SET);
    } else if (cmd == F("left")) {
        simulate5keyRemoteControl(key5Control::SIMULATION_LEFT);
    } else if (cmd == F("right")) {
        simulate5keyRemoteControl(key5Control::SIMULATION_RIGHT);
    } else if (cmd == F("up")) {
        simulate5keyRemoteControl(key5Control::SIMULATION_UP);
    } else if (cmd == F("down")) {
        simulate5keyRemoteControl(key5Control::SIMULATION_DOWN);
    } else if (cmd == F("release")) {
        simulate5keyRemoteControl(key5Control::SIMULATION_RELEASE);
    } else if (cmd == F("open")) {
        OpenClose(true);
    } else if (cmd == F("close")) {
        OpenClose(false);
    }
}

void RunCam::OpenClose(bool open) {
    if (!isConnected)
        return;
    if (!DeviceInfo.hasFeature(Feature::SIMULATE_5_KEY_OSD_CABLE))
        return;
    uint8_t cmd                   = open ? 0x01 : 0x02;
    std::vector<uint8_t> response = sendCommand(Command::KEY5_CONNECTION, std::vector<uint8_t>{cmd});
    if (response.size() != 1) {
        if (getParentPrint() != nullptr)
            println(F("RunCam OpenClose: bad response length."));
        return;
    }
    if (response[0] == ((cmd << 4U) + 1)) {
        is5keyConnected = open;
    } else {
        if (getParentPrint() != nullptr)
            println(F("RunCam OpenClose: Unable to complete operation."));
    }
}

}// namespace obd::video
