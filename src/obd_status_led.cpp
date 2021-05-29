/**
 * \author argawaen 
 * \date 30/05/2021
 *
 */

#include <obd_status_led.h>
#include <obd_system.h>

namespace obd {
namespace core {

void StatusLed::init() {
    pinMode(LED_BUILTIN, OUTPUT);
}

constexpr uint16_t ledHalfPeriod = ledPeriod / 2;
constexpr uint16_t ledQuarterPeriod = ledPeriod / 4;
constexpr uint16_t ledThreeQuarterPeriod = 3 * ledPeriod / 4;
constexpr uint16_t ledEighthPeriod = ledPeriod / 8;
constexpr uint16_t ledThreeEighthPeriod = 3 * ledPeriod / 8;
constexpr uint16_t ledFiveEighthPeriod = 5 * ledPeriod / 8;
constexpr uint16_t ledSevenEighthPeriod = 7 * ledPeriod / 8;

void StatusLed::update() {
    if (getParent()== nullptr)
        return;
    uint64_t delta = getParent()->getTimestamp() - ledTime;
    uint8_t state{0};
    switch (ledState) {
        case LedState::Off:
            break;
        case LedState::Solid:
            state = 1;
            break;
        case LedState::Blink:
            state = (delta < ledHalfPeriod) ? 1 : 0;
            break;
        case LedState::FastBlink:
            if (delta < ledQuarterPeriod) {
                state = 1;
            } else if (delta < ledHalfPeriod) {
                state = 0;
            } else if (delta < ledThreeQuarterPeriod) {
                state = 1;
            }
            break;
        case LedState::TwoPulse:
            if (delta < ledEighthPeriod) {
                state = 1;
            } else if (delta < ledQuarterPeriod) {
                state = 0;
            } else if (delta < ledThreeEighthPeriod) {
                state = 1;
            }
            break;
        case LedState::ThreePulses:
            if (delta < ledEighthPeriod) {
                state = 1;
            } else if (delta < ledQuarterPeriod) {
                state = 0;
            } else if (delta < ledThreeEighthPeriod) {
                state = 1;
            } else if (delta < ledHalfPeriod) {
                state = 0;
            } else if (delta < ledFiveEighthPeriod) {
                state = 1;
            }
            break;
        case LedState::FasterBlink:
            if (delta < ledEighthPeriod) {
                state = 1;
            } else if (delta < ledQuarterPeriod) {
                state = 0;
            } else if (delta < ledThreeEighthPeriod) {
                state = 1;
            } else if (delta < ledHalfPeriod) {
                state = 0;
            } else if (delta < ledFiveEighthPeriod) {
                state = 1;
            } else if (delta < ledThreeQuarterPeriod) {
                state = 0;
            } else if (delta < ledSevenEighthPeriod) {
                state = 1;
            }
            break;
    }
    digitalWrite(LED_BUILTIN, 1 - state);
    if (delta > ledPeriod) {
        ledTime = getParent()->getTimestamp();
    }
}

bool StatusLed::treatCommand(const command &cmd) {
    if (cmd.isCmd("led")) {
        char buf[30];
        strcpy(buf, cmd.getParams());
        if (strcmp(buf, "off") == 0) {
            setState();
        } else if (strcmp(buf, "solid") == 0) {
            setState(LedState::Solid);
        } else if (strcmp(buf, "blink") == 0) {
            setState(LedState::Blink);
        } else if (strcmp(buf, "fastblink") == 0) {
            setState(LedState::FastBlink);
        } else if (strcmp(buf, "twopulse") == 0) {
            setState(LedState::TwoPulse);
        } else if (strcmp(buf, "threepulse") == 0) {
            setState(LedState::ThreePulses);
        } else if (strcmp(buf, "fasterblink") == 0) {
            setState(LedState::FasterBlink);
        } else {
            if (getParent() != nullptr) {
                getParentPrint()->println("Unknown led state");
            }
        }
        return true;
    }
    return false;
}

void StatusLed::setState(LedState st) {
    if (ledState == st)
        return;
    ledState = st;
    if (getParent()== nullptr)
        return;
    ledTime = getParent()->getTimestamp();
}

}// namespace core
}// namespace obd
