/**
 * @file test_led.cpp
 * @author argawaen
 * @date 12/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"
#include "gfx/StatusLed.h"

using namespace obd::gfx;

void test_bad_init() {
    StatusLed led(nullptr);
    led.init();
    TEST_ASSERT_FALSE(led.initialized())
    led.update();
    led.setState(LedState::Solid);
    TEST_ASSERT_FALSE(led.initialized())
}

void test_solid_blink(){
    using obd::core::driver::Message;
    auto led = baseSys.getNode<StatusLed>();
    TEST_ASSERT_NOT_NULL(led);
    TEST_ASSERT(led->pushMessage(Message{0,led->type(),"led off",Message::MessageType::Command}))
    led->update();
    TEST_ASSERT_EQUAL(LedState::Off, led->state());
    TEST_ASSERT(led->pushMessage(Message{0,led->type(),"led solid",Message::MessageType::Command}))
    led->update();
    TEST_ASSERT_EQUAL(LedState::Solid, led->state());
    TEST_ASSERT(led->pushMessage(Message{0,led->type(),"led blink",Message::MessageType::Command}))
    led->update();
    TEST_ASSERT_EQUAL(LedState::Blink, led->state());
    led->accelerateTime(obd::config::ledPeriod/2);
    led->update();
    led->accelerateTime(obd::config::ledPeriod);
    led->update();
    TEST_ASSERT_FALSE(led->pushMessage(Message{0,led->type(),"ledi",Message::MessageType::Command}))
    TEST_ASSERT(led->pushMessage(Message{0,led->type(),"led xmas",Message::MessageType::Command}))
    led->update();
}

void test_fast_blink(){
    using obd::core::driver::Message;
    auto led = baseSys.getNode<StatusLed>();
    TEST_ASSERT_NOT_NULL(led);
    TEST_ASSERT(led->pushMessage(Message{0,led->type(),"led fastblink",Message::MessageType::Command}))
    led->update();
    TEST_ASSERT_EQUAL(LedState::FastBlink, led->state());
    led->accelerateTime(obd::config::ledPeriod/4);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/4);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/4);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/4);
    led->update();
    TEST_ASSERT(led->pushMessage(Message{0,led->type(),"led fasterblink",Message::MessageType::Command}))
    led->update();
    TEST_ASSERT_EQUAL(LedState::FasterBlink, led->state());
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
}

void test_pulse(){
    using obd::core::driver::Message;
    auto led = baseSys.getNode<StatusLed>();
    TEST_ASSERT_NOT_NULL(led);
    TEST_ASSERT(led->pushMessage(Message{0,led->type(),"led twopulse",Message::MessageType::Command}))
    led->update();
    TEST_ASSERT_EQUAL(LedState::TwoPulse, led->state());
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    TEST_ASSERT(led->pushMessage(Message{0,led->type(),"led threepulse",Message::MessageType::Command}))
    led->update();
    TEST_ASSERT_EQUAL(LedState::ThreePulses, led->state());
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
    led->accelerateTime(obd::config::ledPeriod/8);
    led->update();
}

void test_all() {
    UNITY_BEGIN();
    RUN_TEST(test_bad_init);
    RUN_TEST(test_solid_blink);
    RUN_TEST(test_fast_blink);
    RUN_TEST(test_pulse);
    UNITY_END();
}
