/**
 * @file RunCam.cpp
 * @author argawaen
 * @date 11/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "RunCam.h"
#include "native/fakeArduino.h"

namespace obd::camera {

constexpr uint8_t RC_HEADER               = 0xCC;   ///< runCam protocol header
constexpr uint64_t ResponseTimeout        = 500;    ///< Timeout for message reception
constexpr uint64_t ConnexionCheckInterval = 5000000;///< interval between 2 checks for device 5 seconds

void RunCam::init() {
    Node::init();
#ifdef ARDUINO
    uart.begin(115200);
    uart.clearWriteError();
    uart.flush();
#endif
    delay(10);
    getDeviceInfo();
}

OString RunCam::info() const {
    Message msg(0, 0);
    msg.println(F(" ----- RUNCAM INFORMATION -----"));
    msg.print(F("RunCam status: ......................... "));
    switch (status) {
    case Status::READY:
        msg.println("READY");
        break;
    case Status::MENU:
        msg.println("MENU");
        break;
    case Status::DISCONNECTED:
        msg.println("DISCONNECTED");
        break;
    case Status::RECORDING:
        msg.println("RECORDING");
        break;
    case Status::MANUAL:
        msg.println("MANUAL");
        break;
    }
    msg.print(F("Debug print: ........................... "));
    msg.printlnBool(debugPrint);
    if (status != Status::DISCONNECTED) {
        msg.print(F("RunCam Protocol version: ............... "));
        msg.println(DeviceInfo.ProtocolVersion);

        msg.print(F("RunCam Feature Simulate Power Button: .. "));
        msg.printlnBool(DeviceInfo.hasFeature(Feature::SIMULATE_POWER_BUTTON));
        msg.print(F("RunCam Feature Simulate wifi Button: ... "));
        msg.printlnBool(DeviceInfo.hasFeature(Feature::SIMULATE_WIFI_BUTTON));
        msg.print(F("RunCam Feature Change mode: ............ "));
        msg.printlnBool(DeviceInfo.hasFeature(Feature::CHANGE_MODE));
        msg.print(F("RunCam Feature Simulate 5 key osb cable: "));
        msg.printlnBool(DeviceInfo.hasFeature(Feature::SIMULATE_5_KEY_OSD_CABLE));
        msg.print(F("RunCam Feature Device settings access: . "));
        msg.printlnBool(DeviceInfo.hasFeature(Feature::DEVICE_SETTINGS_ACCESS));
        msg.print(F("RunCam Feature display port: ........... "));
        msg.printlnBool(DeviceInfo.hasFeature(Feature::DISPLAY_PORT));
        msg.print(F("RunCam Feature Start recording: ........ "));
        msg.printlnBool(DeviceInfo.hasFeature(Feature::START_RECORDING));
        msg.print(F("RunCam Feature Stop recording: ......... "));
        msg.printlnBool(DeviceInfo.hasFeature(Feature::STOP_RECORDING));
        msg.print(F("RunCam Feature FC attitude: ............ "));
        msg.printlnBool(DeviceInfo.hasFeature(Feature::FC_ATTITUDE));
    } else {
        msg.println(F("Device not connected."));
    }
    return msg.getMessage();
}

void RunCam::preTreatment() {
    // No actions during manual mode
    if (status == Status::MANUAL)
        return;
    // Check for the presence of the device
    chrono += micros() - timestamp;
    timestamp = micros();
    if (chrono > ConnexionCheckInterval) {
        chrono = 0;
        getDeviceInfo();
    }
    // Print any remaining message from the device
#ifdef ARDUINO
    if (uart.available() != 0) {
        Message msg(type(),getConsoleId());
        while (uart.available() != 0) {
            int c = uart.read();
            if (debugPrint) {
                msg.print(c, HEX);
            }
        }
        if (debugPrint) {
            msg.println();
            console(msg.getMessage());
        }
    }
#endif
}

bool RunCam::treatMessage(const Message& message) {
    if (Node::treatMessage(message)) {
        return true;
    }
    if (message.getType() == Message::MessageType::Command) {
        if (message.getBaseCommand() == F("Debug")) {
            debugPrint = !debugPrint;
            return true;
        }
        if (message.getBaseCommand() == F("Test")) {
            getDeviceInfo();
            broadcastMessage(message.getSource(), info(), Message::MessageType::Reply);
            return true;
        }
        if (message.getBaseCommand() == F("Cmd")) {
            parseCmd(message.getParams()[0]);
            return true;
        }
        if (message.getBaseCommand() == F("Menu")) {
            parseMenu(message.getParams()[0]);
            return true;
        }
    }
    return false;
}

bool RunCam::pushCommand(const Message& message) {
    if (Node::pushCommand(message))
        return true;
    if (message.getBaseCommand() == F("Debug")) {
        getMessages().push(message);
        return true;
    }
    if (message.getBaseCommand() == F("Test")) {
        getMessages().push(message);
        return true;
    }
    if (message.getBaseCommand() == F("Cmd")) {
        getMessages().push(message);
        return true;
    }
    if (message.getBaseCommand() == F("Menu")) {
        getMessages().push(message);
        return true;
    }
    return false;
}

void RunCam::getDeviceInfo() {
    std::vector<uint8_t> response = sendCommand(Command::GET_DEVICE_INFO, std::vector<uint8_t>(), true);
    Message msg{type(), 0, Message::MessageType::Reply};
    if (status == Status::DISCONNECTED) {
        msg.println("Camera Disconnected.");
        broadcastMessage(msg);
        return;
    }
    if (response.size() != 3) {
        msg.println(F("RunCam GetInfo: bad response length."));
        broadcastMessage(msg);
        return;
    }
    DeviceInfo.ProtocolVersion = response[0];
    DeviceInfo.Features        = (response[2] << 8U) | response[1];
    if (debugPrint) {
        msg.print(F("RunCam GetInfo message: "));
        for (auto insideChar : response) {
            if (insideChar < 15) msg.print("0");
            msg.print(insideChar, Message::Format::Hexadecimal);
            msg.print(" ");
        }
        msg.println();
        msg.print(F("RunCam GetInfo feature: "));
        msg.println(DeviceInfo.Features, Message::Format::Hexadecimal);
    }
    broadcastMessage(msg);
}

std::vector<uint8_t> RunCam::sendCommand(Command cmd, const std::vector<uint8_t>& params, bool expectResponse) {
    // Only the get info command is allowed if not connected: it allow to determine if the device is connected
    if (status == Status::DISCONNECTED && (cmd != Command::GET_DEVICE_INFO)) {
        console(F("RunCam sendCommand: no connexion for this command."), MessageType::Error);
        return {};
    }
    // creation of the message to send
    std::vector<uint8_t> full_message;
    resetCrc();
    full_message.push_back(RC_HEADER);
    crc8_dvb_s2(RC_HEADER);
    full_message.push_back(static_cast<uint8_t>(cmd));
    crc8_dvb_s2(static_cast<uint8_t>(cmd));
    for (auto param : params) {
        full_message.push_back(param);
        crc8_dvb_s2(param);
    }
    full_message.push_back(current_crc);
#ifdef ARDUINO
    // Effective message send
    uart.write(full_message.data(), full_message.size());
#endif
    full_message.clear();
    if (!expectResponse)
        return {};
    // wait for a response
    delay(10);// wait a little bit
#ifdef ARDUINO
    uint64_t start = millis();
    while (uart.available() == 0) {
        if (millis() - start > ResponseTimeout) {
            // Response timed out: there is maybe no device
            status = Status::DISCONNECTED;
            return std::vector<uint8_t>();
        }
        delay(10);// 10ms
    }
#endif
    // read the message
    resetCrc();
#ifdef ARDUINO
    bool valid = false;
    bool msg   = false;
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
        console(F("RunCam sendCommand: bad CRC."));
        return {};
    }
#endif
    // active connexion
    if (cmd == Command::GET_DEVICE_INFO && status == Status::DISCONNECTED)
        status = Status::READY;
    if (debugPrint) {
        Message msag(type(), 0, MessageType::Reply);
        msag.print(F("RunCam sendCommand message: "));
        for (auto insideChar : full_message) {
            msag.print(insideChar, Message::Format::Hexadecimal);
            msag.print(" ");
        }
        msag.println();
        broadcastMessage(msag);
    }
    // Return message content
    return full_message;
}

void RunCam::parseCmd(const OString& cmd) {
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
        console(F("Unknown Camera Command"), MessageType::Error);
    }
}

void RunCam::parseMenu(const OString& cmd) {
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
        console(F("Unknown Menu Command"), Message::MessageType::Error);
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
        console(F("Move impossible: not in Menu"), MessageType::Error);
        return;
    }
    if (DeviceInfo.hasFeature(Feature::SIMULATE_5_KEY_OSD_CABLE)) {
        sendCommand(
                Command::KEY5_SIMULATION_PRESS,
                {static_cast<uint8_t>(dir)});
        if (navMenu.moveMenu(dir))
            status = Status::READY;
    } else {
        console(F("Move impossible: no such feature"), MessageType::Error);
    }
}

}// namespace obd::camera
