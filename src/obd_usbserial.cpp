/**
 * \author argawaen 
 * \date 30/05/2021
 *
 */

#include <obd_system.h>
#include <obd_usbserial.h>

namespace obd {
namespace core {

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

void UsbSerial::update(uint64_t  /*timestamp*/) {
    if (Serial.available() > 0) {
        delay(10);
        command cmd(source::USB);
        while (Serial.available() > 0) {
            char c = Serial.read();
            if (c == '\n') break;
            if (!cmd.putChar(c)) break;
        }
        if (getParent() != nullptr) {
            getParent()->pushCommand(cmd);
        }
    }
}

void UsbSerial::printInfo() {
    if (getParent() != nullptr) {
        getParent()->printKernelInfo();
    }
}


}// namespace core
}// namespace obd
