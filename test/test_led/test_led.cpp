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
    TEST_ASSERT_FALSE(led.init())
    led.printHelp();
    led.update(15);
    TEST_ASSERT_FALSE(led.treatCommand(obd::core::Command()))
    led.setState(LedState::Solid);
    TEST_ASSERT_FALSE(led.initialized())
}

void test_solid_blink(){
    auto led = baseSys.getDriver<StatusLed>();
    led->printHelp();
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led off"}))
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led"}))
    led->update(50);
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led solid"}))
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led"}))
    led->update(50);
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led blink"}))
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led"}))
    led->update(50);
    led->update(obd::config::ledPeriod/2);
    led->update(obd::config::ledPeriod);
    TEST_ASSERT_FALSE(led->treatCommand(obd::core::Command{"ledi"}))
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led noel"}))
}

void test_fast_blink(){
    auto led = baseSys.getDriver<StatusLed>();
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led fastblink"}))
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led"}))
    led->update(50);
    led->update(obd::config::ledPeriod/4);
    led->update(obd::config::ledPeriod/4);
    led->update(obd::config::ledPeriod/4);
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led fasterblink"}))
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led"}))
    led->update(50);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
}


void test_pulse(){
    auto led = baseSys.getDriver<StatusLed>();
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led twopulse"}))
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led"}))
    led->update(50);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led threepulse"}))
    TEST_ASSERT(led->treatCommand(obd::core::Command{"led"}))
    led->update(50);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
    led->update(obd::config::ledPeriod/8);
}

void test_all() {
    UNITY_BEGIN();
    RUN_TEST(test_bad_init);
    RUN_TEST(test_solid_blink);
    RUN_TEST(test_fast_blink);
    RUN_TEST(test_pulse);
    UNITY_END();
}
