
#include "obd_system.h"

void setup() {
    obd::system.init();
}

void loop() {
    obd::system.update();
}