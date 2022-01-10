/**
 * @file StatusLed.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "native/fakeArduino.h"
#include "StatusLed.h"
#include "core/System.h"

namespace obd::config {
constexpr uint64_t ledHalfPeriod         = ledPeriod / 2;    ///< half period time
constexpr uint64_t ledQuarterPeriod      = ledPeriod / 4;    ///< quarter period time
constexpr uint64_t ledThreeQuarterPeriod = 3 * ledPeriod / 4;///< 3 quarters period time
constexpr uint64_t ledEighthPeriod       = ledPeriod / 8;    ///< Eighth period time
constexpr uint64_t ledThreeEighthPeriod  = 3 * ledPeriod / 8;///< 3 Eighth period time
constexpr uint64_t ledFiveEighthPeriod   = 5 * ledPeriod / 8;///< 5 Eighth period time
constexpr uint64_t ledSevenEighthPeriod  = 7 * ledPeriod / 8;///< 7 Eighth period time
}// namespace obd::config

namespace obd::gfx {

bool StatusLed::init() {
#ifdef ARDUINO
    pinMode(LED_BUILTIN, OUTPUT);
#endif
    return true;
}

void StatusLed::update(int64_t delta) {if (getParent() == nullptr)
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
#ifdef ARDUINO
    digitalWrite(LED_BUILTIN, static_cast<uint8_t>(1U - state));
#else
    if (state == 1) {

    }
#endif
    if (ledTime > config::ledPeriod) {
        ledTime = 0;
    }
}

bool StatusLed::treatCommand(const core::Command& cmd) {
    if (cmd.isCmd(F("led"))) {
        std::string buf{cmd.getParams()};
        if (buf.empty()) {
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
                getParentOutput()->println(F("Unknown led state"));
            }
        }
        return true;
    }
    return false;
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

void StatusLed::setState(LedState newState) {
    if (ledState == newState)
        return;
    ledState = newState;
    if (getParent() == nullptr)
        return;
    ledTime = getParent()->getTimestamp();
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

uint8_t StatusLed::fastBlinkCb() const {
    if (ledTime < config::ledQuarterPeriod)
        return 1U;
    if (ledTime < config::ledHalfPeriod)
        return 0;
    if (ledTime < config::ledThreeQuarterPeriod)
        return 1U;
    return 0;
}

uint8_t StatusLed::twoPulsesCb() const {
    if (ledTime < config::ledEighthPeriod)
        return 1U;
    if (ledTime < config::ledQuarterPeriod)
        return 0;
    if (ledTime < config::ledThreeEighthPeriod)
        return 1U;
    return 0;
}

uint8_t StatusLed::threePulsesCb() const {
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

uint8_t StatusLed::fasterBlinkCb() const {
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

}// namespace obd::gfx
