/**
 * @file Stdout.cpp
 * @author argawaen
 * @date 19/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Stdout.h"
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <iostream>
#endif

namespace obd::com {

void Stdout::writeLine(const OString& line) {
#ifdef ARDUINO
    Serial.println(line);
#else
    std::cout << line << std::endl;
#endif
}
bool Stdout::available() {
#ifdef ARDUINO
    return Serial.available()>0;
#else
    return false;
#endif
}
OString Stdout::readLine() {
#ifdef ARDUINO
    OString input;
    while(Serial.available()) {
        input += static_cast<char>(Serial.read());
        delay(1);
        if (input.back() == '\n') {
            input.pop_back();
            break;
        }
    }
    return input;
#else
    return {};
#endif
}
void Stdout::init() {
    Node::init();
#ifdef ARDUINO
    Serial.begin(115200);
    Serial.println();
#endif
}

}// namespace obd::com
