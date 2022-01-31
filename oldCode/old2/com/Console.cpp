/**
 * @file Console.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Console.h"
#include "core/System_old.h"
#include "native/fakeArduino.h"

namespace obd::com {

bool Console::init() {
#ifdef ARDUINO
    Serial.begin(115200);
    uint32_t br = Serial.detectBaudrate(1000);
    if (br != 0U) {
        Serial.updateBaudRate(br);
    }
#endif
    _output = std::make_shared<ConsoleOutput>();
    addOutput(_output);
    println();
    println(F("SYSTEM INIT"));
    return core::BaseDriver::init();
}

void Console::printInfo() {}

void Console::update([[maybe_unused]] int64_t delta) {
#ifdef ARDUINO
    if (Serial.available() > 0) {
        delay(10);
        core::command cmd(source::USB);
        while (Serial.available() > 0) {
            char c = static_cast<char>(Serial.read());
            if (c == '\n') break;
            if (c == '\r') break;
            if (!cmd.putChar(c)) break;
        }
        if (cmd.empty())
            return;
        pushCommand(cmd);
    }
#endif
}

}// namespace obd::com
