
#include "obd_system.h"

void setup() {
    obd::hardware.init();
}

void loop() {
    obd::hardware.update();
}