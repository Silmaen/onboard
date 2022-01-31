/**
 * @file ConsoleOutput.cpp
 * @author argawaen
 * @date 13/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "ConsoleOutput.h"
#ifdef Arduino
#include <Arduino.h>
#else
#include <iostream>
#endif

namespace obd::com {

void ConsoleOutput::writeBytes(const OString& data) {
#ifdef ARDUINO
    Serial.print(data.c_str);
#else
    std::cout << data;
#endif
}

}// namespace obd::com