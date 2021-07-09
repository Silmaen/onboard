/**
 * @author Silmaen
 * @date 30/05/2021
 */

#include <Arduino.h>
#include <obd_status_led.h>
#include <obd_system.h>

namespace obd::config {
constexpr uint16_t ledHalfPeriod         = ledPeriod / 2;    ///< half period time
constexpr uint16_t ledQuarterPeriod      = ledPeriod / 4;    ///< quarter period time
constexpr uint16_t ledThreeQuarterPeriod = 3 * ledPeriod / 4;///< 3 quarters period time
constexpr uint16_t ledEighthPeriod       = ledPeriod / 8;    ///< Eighth period time
constexpr uint16_t ledThreeEighthPeriod  = 3 * ledPeriod / 8;///< 3 Eighth period time
constexpr uint16_t ledFiveEighthPeriod   = 5 * ledPeriod / 8;///< 5 Eighth period time
constexpr uint16_t ledSevenEighthPeriod  = 7 * ledPeriod / 8;///< 7 Eighth period time
}// namespace obd::config

namespace obd::core {

void StatusLed::init() {
    pinMode(LED_BUILTIN, OUTPUT);
}

uint8_t StatusLed::fastBlinkCb() const {
    if (ledTime < config::ledQuarterPeriod)
        return 1U;
    if (ledTime < config::ledHalfPeriod)
        return 0;
    if (ledTime < config::ledThreeQuarterPeriod)
        return 1U;
    return 0;
}

[[nodiscard]] uint8_t StatusLed::twoPulsesCb() const {
    if (ledTime < config::ledEighthPeriod)
        return 1U;
    if (ledTime < config::ledQuarterPeriod)
        return 0;
    if (ledTime < config::ledThreeEighthPeriod)
        return 1U;
    return 0;
}

[[nodiscard]] uint8_t StatusLed::threePulsesCb() const {
    if (ledTime < config::ledEighthPeriod)
        return 1U;
    if (ledTime < config::ledQuarterPeriod)
        return 0;
    if (ledTime < config::ledThreeEighthPeriod)
        return 1U;
    if (ledTime < config::ledHalfPeriod)
        return 0U;
    if (ledTime < config::ledFiveEighthPeriod)
        return 1U;
    return 0;
}

[[nodiscard]] uint8_t StatusLed::fasterBlinkCb() const {
    if (ledTime < config::ledEighthPeriod)
        return 1U;
    if (ledTime < config::ledQuarterPeriod)
        return 0;
    if (ledTime < config::ledThreeEighthPeriod)
        return 1U;
    if (ledTime < config::ledHalfPeriod)
        return 0;
    if (ledTime < config::ledFiveEighthPeriod)
        return 1U;
    if (ledTime < config::ledThreeQuarterPeriod)
        return 0;
    if (ledTime < config::ledSevenEighthPeriod)
        return 1U;
    return 0;
}

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
        state = static_cast<uint8_t>((ledTime < config::ledHalfPeriod) ? 1U : 0U);
        break;
    case LedState::FastBlink:
        state = fastBlinkCb();
        break;
    case LedState::TwoPulse:
        state = twoPulsesCb();
        break;
    case LedState::ThreePulses:
        state = threePulsesCb();
        break;
    case LedState::FasterBlink:
        state = fasterBlinkCb();
    }
    digitalWrite(LED_BUILTIN, static_cast<uint8_t>(1U - state));
    if (ledTime > config::ledPeriod) {
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
    println(F("Help on led state"));
    println(F("led           Print the current LED status"));
    println(F("led <state>   Change the state of the led. valid state are:"));
    println(F("              off         led off"));
    println(F("              solid       led on"));
    println(F("              blink       led is slowly blinking"));
    println(F("              fastblink   led is blinking twice faster"));
    println(F("              twopulse    led do 2 pulses then wait"));
    println(F("              threepulse  led do 3 pulses then wait"));
    println(F("              fasterblink lest is continuously pulsing"));
}

void StatusLed::printCurrentState() {
    if (getParent() == nullptr)
        return;
    switch (ledState) {
    case LedState::Off:
        println(F("LED state: off"));
        break;
    case LedState::Solid:
        println(F("LED state: solid"));
        break;
    case LedState::Blink:
        println(F("LED state: blink"));
        break;
    case LedState::FastBlink:
        println(F("LED state: fastblink"));
        break;
    case LedState::TwoPulse:
        println(F("LED state: twopulse"));
        break;
    case LedState::ThreePulses:
        println(F("LED state: threepulse"));
        break;
    case LedState::FasterBlink:
        println(F("LED state: fasterblink"));
        break;
    }
}

}// namespace obd::core
