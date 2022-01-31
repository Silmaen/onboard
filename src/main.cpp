
#include "native/fakeArduino.h"
#include "gfx/StatusLed.h"
//#include "gfx/Display.h"
#include <core/System.h>
static obd::core::System hardware;

void setup() {
#if defined(ONBOARD)
#elif defined(REMOTE)
    //hardware.addNode<obd::gfx::Display>();
#elif defined(NATIVE)
#else
#error "Unsupported Pio Environment"
#endif
    hardware.init();
}

void loop() {
    hardware.update();
}