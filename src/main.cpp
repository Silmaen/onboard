
#if defined(ONBOARD)
#include <Arduino.h>
#include <obd_system.h>

static obd::core::System hardware;

void setup() {
    hardware.init();
}

void loop() {
    hardware.update();
}
#elif defined(REMOTE)
void setup() {

}

void loop() {

}
#elif defined(NATIVE)
#include "native/fakeArduino.h"
void setup() {

}

void loop() {

}
#else
#error "Unsupported Pio Environment"
#endif