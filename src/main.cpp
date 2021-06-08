
#include <Arduino.h>
#include "obd_system.h"

static obd::core::system hardware;

void setup() {
    hardware.init();
}

void loop() {
    hardware.update();
}