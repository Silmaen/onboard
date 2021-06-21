
#include <Arduino.h>
#include <obd_system.h>
#include <obd_systemtime.h>

static obd::core::system hardware;

namespace obd::time {
time_t timeCb() {
    auto cl = hardware.getDriverAs<clock>("SystemClock");
    if (cl != nullptr)
        return cl->getDate();
    return 0;
}
}// namespace obd::time

void setup() {
    hardware.init();
}

void loop() {
    hardware.update();
}