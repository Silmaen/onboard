/**
 * @author Silmaen
 * @date 30/05/2021
 */

#include <Arduino.h>
#include <obd_status_led.h>
#include <obd_system.h>
namespace obd::core {

void StatusLed::init() {
    pinMode(LED_BUILTIN, OUTPUT);
}

constexpr uint16_t ledHalfPeriod         = ledPeriod / 2;
constexpr uint16_t ledQuarterPeriod      = ledPeriod / 4;
constexpr uint16_t ledThreeQuarterPeriod = 3 * ledPeriod / 4;
constexpr uint16_t ledEighthPeriod       = ledPeriod / 8;
constexpr uint16_t ledThreeEighthPeriod  = 3 * ledPeriod / 8;
constexpr uint16_t ledFiveEighthPeriod   = 5 * ledPeriod / 8;
constexpr uint16_t ledSevenEighthPeriod  = 7 * ledPeriod / 8;

void StatusLed::update(int64_t delta) {
    if (getParent() == nullptr)
        return;
    ledTime += delta;
    uint8_t state{0};
    switch (ledState) {
    case LedState::Off:
        break;
    case LedState::Solid:
        state = 1;
        break;
    case LedState::Blink:
        state = static_cast<uint8_t>((ledTime < ledHalfPeriod) ? 1U : 0U);
        break;
    case LedState::FastBlink:
        if (ledTime < ledQuarterPeriod) {
            state = 1U;
        } else if (ledTime < ledHalfPeriod) {
            state = 0;
        } else if (ledTime < ledThreeQuarterPeriod) {
            state = 1U;
        }
        break;
    case LedState::TwoPulse:
        if (ledTime < ledEighthPeriod) {
            state = 1U;
        } else if (ledTime < ledQuarterPeriod) {
            state = 0;
        } else if (ledTime < ledThreeEighthPeriod) {
            state = 1U;
        }
        break;
    case LedState::ThreePulses:
        if (ledTime < ledEighthPeriod) {
            state = 1U;
        } else if (ledTime < ledQuarterPeriod) {
            state = 0;
        } else if (ledTime < ledThreeEighthPeriod) {
            state = 1U;
        } else if (ledTime < ledHalfPeriod) {
            state = 0U;
        } else if (ledTime < ledFiveEighthPeriod) {
            state = 1U;
        }
        break;
    case LedState::FasterBlink:
        if (ledTime < ledEighthPeriod) {
            state = 1U;
        } else if (ledTime < ledQuarterPeriod) {
            state = 0;
        } else if (ledTime < ledThreeEighthPeriod) {
            state = 1U;
        } else if (ledTime < ledHalfPeriod) {
            state = 0;
        } else if (ledTime < ledFiveEighthPeriod) {
            state = 1U;
        } else if (ledTime < ledThreeQuarterPeriod) {
            state = 0;
        } else if (ledTime < ledSevenEighthPeriod) {
            state = 1U;
        }
        break;
    }
    digitalWrite(LED_BUILTIN, static_cast<uint8_t>(1U - state));
    if (ledTime > ledPeriod) {
        ledTime = 0;
    }
}

bool StatusLed::treatCommand(const command& cmd) {
    if (cmd.isCmd(F("led"))) {
        String buf{cmd.getParams()};
        if (buf.isEmpty()) {
            printCurrentState();
        } else if (buf == F("off")) {
            setState();
        } else if (buf == F("solid")) {
            setState(LedState::Solid);
        } else if (buf == F("blink")) {
            setState(LedState::Blink);
        } else if (buf == F("fastblink")) {
            setState(LedState::FastBlink);
        } else if (buf == F("twopulse")) {
            setState(LedState::TwoPulse);
        } else if (buf == F("threepulse")) {
            setState(LedState::ThreePulses);
        } else if (buf == F("fasterblink")) {
            setState(LedState::FasterBlink);
        } else {
            if (getParent() != nullptr) {
                getParentPrint()->println(F("Unknown led state"));
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
    if (getParent() == nullptr)
        return;
    ledTime = getParent()->getTimestamp();
}

void StatusLed::printHelp() {
    if (getParent() == nullptr)
        return;
    getParentPrint()->println(F("Help on led state"));
    getParentPrint()->println(F("led           Print the current LED status"));
    getParentPrint()->println(F("led <state>   Change the state of the led. valid state are:"));
    getParentPrint()->println(F("              off         led off"));
    getParentPrint()->println(F("              solid       led on"));
    getParentPrint()->println(F("              blink       led is slowly blinking"));
    getParentPrint()->println(F("              fastblink   led is blinking twice faster"));
    getParentPrint()->println(F("              twopulse    led do 2 pulses then wait"));
    getParentPrint()->println(F("              threepulse  led do 3 pulses then wait"));
    getParentPrint()->println(F("              fasterblink lest is continuously pulsing"));
}

void StatusLed::printCurrentState() {
    if (getParent() == nullptr)
        return;
    switch (ledState) {
    case LedState::Off:
        getParentPrint()->println(F("LED state: off"));
        break;
    case LedState::Solid:
        getParentPrint()->println(F("LED state: solid"));
        break;
    case LedState::Blink:
        getParentPrint()->println(F("LED state: blink"));
        break;
    case LedState::FastBlink:
        getParentPrint()->println(F("LED state: fastblink"));
        break;
    case LedState::TwoPulse:
        getParentPrint()->println(F("LED state: twopulse"));
        break;
    case LedState::ThreePulses:
        getParentPrint()->println(F("LED state: threepulse"));
        break;
    case LedState::FasterBlink:
        getParentPrint()->println(F("LED state: fasterblink"));
        break;
    }
}

}// namespace obd::core
