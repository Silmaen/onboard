/**
 * @file StatusLed.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "StatusLed.h"
#include "native/fakeArduino.h"
#include "config.h"

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

void StatusLed::init() {
    Node::init();
#ifdef ARDUINO
    pinMode(LED_BUILTIN, OUTPUT);
#endif
}

void StatusLed::preTreatment() {
    ledTime += micros() - timestamp;
    timestamp = micros();
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
    while (ledTime > config::ledPeriod) {
        ledTime -= config::ledPeriod;
    }
}

bool StatusLed::treatMessage(const Message& cmd){
    if (Node::treatMessage(cmd))
        return true;
    if (cmd.getBaseCommand()==F("led")) {
        if (!cmd.hasParams()) {
            printCurrentState();
        } else {
            OString buf{cmd.getParams()[0]};
            if (buf == F("off")) {
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
                console("Unknown led State", Message::MessageType::Error);
            }
        }
        return true;
    }
    return false;
}

void StatusLed::setState(LedState newState) {
    if (!initialized())
        return;
    if (ledState == newState)
        return;
    ledState = newState;
    ledTime = 0;
}

void StatusLed::printCurrentState() {
    Message msg(type(), getConsoleId());
    switch (ledState) {
    case LedState::Off:
        msg.println(F("LED state: off"));
        break;
    case LedState::Solid:
        msg.println(F("LED state: solid"));
        break;
    case LedState::Blink:
        msg.println(F("LED state: blink"));
        break;
    case LedState::FastBlink:
        msg.println(F("LED state: fastblink"));
        break;
    case LedState::TwoPulse:
        msg.println(F("LED state: twopulse"));
        break;
    case LedState::ThreePulses:
        msg.println(F("LED state: threepulse"));
        break;
    case LedState::FasterBlink:
        msg.println(F("LED state: fasterblink"));
        break;
    }
    broadcastMessage(msg);
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

void StatusLed::accelerateTime(uint64_t addedTime) {
    ledTime += addedTime;
}

bool StatusLed::pushCommand(const core::driver::Message& message) {
    if (Node::pushCommand(message))
        return true;
    if (message.getBaseCommand() == F("led")) {
        getMessages().push(message);
        return true;
    }
    return false;
}

}// namespace obd::gfx
