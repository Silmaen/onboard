
#include "native/fakeArduino.h"
#include <core/System.h>
static obd::core::System hardware;

void setup() {
#if defined(ONBOARD)
#elif defined(REMOTE)
#elif defined(NATIVE)
#else
#error "Unsupported Pio Environment"
#endif
hardware.init();
}

void loop() {
    hardware.update();
}