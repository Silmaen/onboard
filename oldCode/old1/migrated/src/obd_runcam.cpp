/**
 * @author Silmaen
 * @date 01/07/2021.
 */

#include "obd_runcam.h"
#include <Arduino.h>

namespace obd::video {

constexpr uint8_t RC_HEADER               = 0xCC;   ///< runCam protocol header
constexpr uint64_t ResponseTimeout        = 500;    ///< Timeout for message reception
constexpr uint64_t ConnexionCheckInterval = 5000000;///< interval between 2 checks for device 5 seconds

void RunCam::init() {
    uart.begin(115200);
    uart.clearWriteError();
    uart.flush();
    delay(10);
    getDeviceInfo();
}

void RunCam::printInfo() {
    println(F(" ----- RUNCAM INFORMATION -----"));
    print(F("RunCam status: ......................... "));
    switch (status) {
    case Status::READY:
        println("READY");
        break;
    case Status::MENU:
        println("MENU");
        break;
    case Status::DISCONNECTED:
        println("DISCONNECTED");
        break;
    case Status::RECORDING:
        println("RECORDING");
        break;
    case Status::MANUAL:
        println("MANUAL");
        break;
    }
    print(F("Debug print: ........................... "));
    printlnBool(debugPrint);
    if (status != Status::DISCONNECTED) {
        print(F("RunCam Protocol version: ............... "));
        println(DeviceInfo.ProtocolVersion);

        print(F("RunCam Feature Simulate Power Button: .. "));
        printlnBool(DeviceInfo.hasFeature(Feature::SIMULATE_POWER_BUTTON));
        print(F("RunCam Feature Simulate wifi Button: ... "));
        printlnBool(DeviceInfo.hasFeature(Feature::SIMULATE_WIFI_BUTTON));
        print(F("RunCam Feature Change mode: ............ "));
        printlnBool(DeviceInfo.hasFeature(Feature::CHANGE_MODE));
        print(F("RunCam Feature Simulate 5 key osb cable: "));
        printlnBool(DeviceInfo.hasFeature(Feature::SIMULATE_5_KEY_OSD_CABLE));
        print(F("RunCam Feature Device settings access: . "));
        printlnBool(DeviceInfo.hasFeature(Feature::DEVICE_SETTINGS_ACCESS));
        print(F("RunCam Feature display port: ........... "));
        printlnBool(DeviceInfo.hasFeature(Feature::DISPLAY_PORT));
        print(F("RunCam Feature Start recording: ........ "));
        printlnBool(DeviceInfo.hasFeature(Feature::START_RECORDING));
        print(F("RunCam Feature Stop recording: ......... "));
        printlnBool(DeviceInfo.hasFeature(Feature::STOP_RECORDING));
        print(F("RunCam Feature FC attitude: ............ "));
        printlnBool(DeviceInfo.hasFeature(Feature::FC_ATTITUDE));
    } else {
        println(F("Device not connected."));
    }
}

void RunCam::update(int64_t delta) {
    // No actions during manual mode
    if (status == Status::MANUAL)
        return;
    // Check for the presence of the device
    chrono += delta;
    if (chrono > ConnexionCheckInterval) {
        chrono = 0;
        getDeviceInfo();
    }
    // Print any remaining message from the device
    if (uart.available() != 0) {
        if (debugPrint) {
            print("runcam > ");
        }
        while (uart.available() != 0) {
            int c = uart.read();
            if (debugPrint) {
                if (c < 15) print('0');
                print(c, HEX);
            }
        }
        if (debugPrint) {
            println();
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
        getDeviceInfo();
        printInfo();
        return true;
    }
    if (cmd.isCmd(F("runcamCmd"))) {
        parseCmd(cmd.getParams());
        return true;
    }
    if (cmd.isCmd(F("runcamMenu"))) {
        parseMenu(cmd.getParams());
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
    println(F("                manual Set the device in an uncontrolled mode"));
    println(F("                ctrl   Set the device in a controlled mode"));
    println(F("                reset  Reset the internal state of the driver,"));
    println(F("                       Assuming the camera is in READY state"));
    println(F("                start  Start Camera"));
    println(F("                stop   Stop Camera"));
    println(F("runcamMenu     Send a menu action to the connected device. Valid parameters are:"));
    println(F("                open    Open the menu"));
    println(F("                set     Simulate the push on button"));
    println(F("                left    Simulate the left direction"));
    println(F("                right   Simulate the right direction"));
    println(F("                up      Simulate the up direction"));
    println(F("                down    Simulate the down direction"));
}

void RunCam::loadConfigFile() {
}

void RunCam::saveConfigFile() const {
}

void RunCam::getDeviceInfo() {
    std::vector<uint8_t> response = sendCommand(Command::GET_DEVICE_INFO, std::vector<uint8_t>(), true);
    if (status == Status::DISCONNECTED)
        return;
    if (response.size() != 3) {
        println(F("RunCam GetInfo: bad response length."));
        return;
    }
    DeviceInfo.ProtocolVersion = response[0];
    DeviceInfo.Features        = (response[2] << 8U) | response[1];
    if (debugPrint){
        print(F("RunCam GetInfo message: "));
        for (auto c : response) {
            if (c<15) print("0");
            print(c, HEX);
            print(" ");
        }
        println();
        print(F("RunCam GetInfo feature: "));
        println(DeviceInfo.Features, HEX);
    }
}

std::vector<uint8_t> RunCam::sendCommand(Command cmd, const std::vector<uint8_t>& params, bool expectResponse) {
    // Only the get info command is allowed if not connected: it allow to determine if the device is connected
    if (status == Status::DISCONNECTED && (cmd != Command::GET_DEVICE_INFO)) {
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
    // Effective message send
    uart.write(full_message.data(), full_message.size());
    full_message.clear();
    if (!expectResponse)
        return std::vector<uint8_t>();
    // wait for a response
    delay(10);// wait a little bit
    uint64_t start = millis();
    while (uart.available() == 0) {
        if (millis() - start > ResponseTimeout) {
            // Response timed out: there is maybe no device
            status = Status::DISCONNECTED;
            return std::vector<uint8_t>();
        }
        delay(10);// 10ms
    }
    // read the message
    resetCrc();
    bool msg   = false;
    bool valid = false;
    while (uart.available() != 0) {
        uint8_t c = uart.read();
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
        delay(10);// Slow down a bit the reading!
    }
    // Message verification
    if (!valid) {
        println(F("RunCam sendCommand: bad CRC."));
        return std::vector<uint8_t>();
    }
    // active connexion
    if (cmd == Command::GET_DEVICE_INFO && status == Status::DISCONNECTED)
        status = Status::READY;
    if (debugPrint) {
        print(F("RunCam sendCommand message: "));
        for (auto c : full_message) {
            if (c<15) print("0");
            print(c, HEX);
            print(" ");
        }
        println();
    }
    // Return message content
    return full_message;
}

void RunCam::parseCmd(const String& cmd) {
    if (cmd == F("manual")) {
        setManual();
    } else if (cmd == F("ctrl")) {
        unsetManual();
    } else if (cmd == F("reset")) {
        resetState();
    } else if (cmd == F("start")) {
        startRecording();
    } else if (cmd == F("stop")) {
        stopRecording();
    } else {
        println(F("Unknown Camera Command"));
    }
}

void RunCam::parseMenu(const String& cmd) {
    if (cmd == F("open")) {
        openMenu();
    } else if (cmd == F("set")) {
        moveSet();
    } else if (cmd == F("left")) {
        moveLeft();
    } else if (cmd == F("right")) {
        moveRight();
    } else if (cmd == F("up")) {
        moveUp();
    } else if (cmd == F("down")) {
        moveDown();
    } else {
        println(F("Unknown Menu Command"));
    }
}

void RunCam::setManual() {
    if (status == Status::DISCONNECTED)
        return;
    status = Status::MANUAL;
}

void RunCam::unsetManual() {
    if (status != Status::MANUAL)
        return;
    status = Status::READY;
}

void RunCam::resetState() {
    if (status == Status::DISCONNECTED)
        return;
    status = Status::READY;
}

void RunCam::startRecording() {
    if (status != Status::READY)
        return;
    if (DeviceInfo.hasFeature(Feature::START_RECORDING)) {
        sendCommand(Command::CAMERA_CONTROL, {static_cast<uint8_t>(ControlCommand::CHANGE_START_RECORDING)}, false);
        status = Status::RECORDING;
    } else if (DeviceInfo.hasFeature(Feature::SIMULATE_POWER_BUTTON)) {
        sendCommand(Command::CAMERA_CONTROL, {static_cast<uint8_t>(ControlCommand::SIMULATE_POWER_BTN)}, false);
        status = Status::RECORDING;
    }
}

void RunCam::stopRecording() {
    if (status != Status::RECORDING)
        return;
    if (DeviceInfo.hasFeature(Feature::STOP_RECORDING)) {
        sendCommand(
                Command::CAMERA_CONTROL,
                {static_cast<uint8_t>(ControlCommand::CHANGE_STOP_RECORDING)},
                false);
        status = Status::READY;
    } else if (DeviceInfo.hasFeature(Feature::SIMULATE_POWER_BUTTON)) {
        sendCommand(
                Command::CAMERA_CONTROL,
                {static_cast<uint8_t>(ControlCommand::SIMULATE_POWER_BTN)},
                false);
        status = Status::READY;
    }
}

void RunCam::openMenu() {
    if (status != Status::READY)
        return;
    if (!DeviceInfo.hasFeature(Feature::CHANGE_MODE)) {
        sendCommand(Command::CAMERA_CONTROL,
                    {static_cast<uint8_t>(ControlCommand::CHANGE_MODE)},
                    false);
        status = Status::MENU;
        navMenu.enterMenu();
    } else if (DeviceInfo.hasFeature(Feature::SIMULATE_5_KEY_OSD_CABLE)) {
        sendCommand(
                Command::KEY5_SIMULATION_PRESS,
                {static_cast<uint8_t>(NavDirection::Enter)});
        status = Status::MENU;
        navMenu.enterMenu();
    }
}

void RunCam::moveLeft() {
    moveMenu(NavDirection::Left);
}

void RunCam::moveRight() {
    moveMenu(NavDirection::Right);
}

void RunCam::moveSet() {
    moveMenu(NavDirection::Enter);
}

void RunCam::moveUp() {
    moveMenu(NavDirection::Up);
}

void RunCam::moveDown() {
    moveMenu(NavDirection::Down);
}

void RunCam::moveMenu(const NavDirection& dir) {
    if (status != Status::MENU) {
        println(F("Move impossible: not in Menu"));
        return;
    }
    if (DeviceInfo.hasFeature(Feature::SIMULATE_5_KEY_OSD_CABLE)) {
        sendCommand(
                Command::KEY5_SIMULATION_PRESS,
                {static_cast<uint8_t>(dir)});
        if (navMenu.moveMenu(dir))
            status = Status::READY;
    }else{
        println(F("Move impossible: no such feature"));
    }
}

}// namespace obd::video
