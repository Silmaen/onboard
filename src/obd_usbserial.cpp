/**
 * @author Silmaen
 * @date 30/05/2021
 */

#include "Arduino.h"
#include <obd_system.h>
#include <obd_usbserial.h>

namespace obd::network {

void UsbSerial::init() {
    Serial.begin(115200);
    uint32_t br = Serial.detectBaudrate(1000);
    if (br != 0U) {
        Serial.updateBaudRate(br);
    }
    if (getParent() != nullptr) {
        getParent()->addPrint(&Serial);
        Serial.println();
        Serial.println(F("SYSTEM INIT"));
    }
}

void UsbSerial::update(int64_t /*timestamp*/) {
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
}

void UsbSerial::printInfo() {}

}// namespace obd::network
