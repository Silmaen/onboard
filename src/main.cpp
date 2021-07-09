
#include <Arduino.h>
#include <obd_system.h>

static obd::core::System hardware;

void setup() {
    hardware.init();
}

void loop() {
    hardware.update();
}