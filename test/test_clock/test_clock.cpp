/**
 * @file test_clock.cpp
 * @author argawaen
 * @date 11/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"
#include "time/Clock.h"

void test_bad_init(){
    auto badClock = obd::time::Clock(nullptr);
    TEST_ASSERT_FALSE(badClock.init())
    auto system = obd::core::System();
    auto badClock2 = obd::time::Clock(&system);
    TEST_ASSERT(badClock2.init())
}

void test_update(){
    auto clk = baseSys.getDriver<obd::time::Clock>();
    auto hdd = baseSys.getDriver<obd::fs::FileSystem>();
    obd::fs::Path time_save{obd::config::tsSave};
    TEST_ASSERT(hdd->mkdir(time_save.parent(),true,true))
    clk->update(obd::config::saveInterval + 1); // try to provoque a time save
    TEST_ASSERT(hdd->exists(obd::fs::Path{obd::config::tsSave}))
}

#include <iostream>
void test_commands(){
    auto clk = baseSys.getDriver<obd::time::Clock>();
    clk->printHelp();
    TEST_ASSERT(clk->treatCommand(obd::core::Command{"date"}))
    TEST_ASSERT(clk->treatCommand(obd::core::Command{"clockinfo"}))
    auto pool = clk->getPoolServer();
    auto timeZone = clk->getTimeZone();
    std::cout << "read time zone: " << timeZone << std::endl;
    TEST_ASSERT(clk->treatCommand(obd::core::Command{"clockpool boby"}))
    TEST_ASSERT(clk->treatCommand(obd::core::Command{"clocktz toto"}))
    TEST_ASSERT_EQUAL_STRING("boby",clk->getPoolServer().c_str());
    TEST_ASSERT_EQUAL_STRING("toto",clk->getTimeZone().c_str());
    TEST_ASSERT(clk->treatCommand(obd::core::Command{"clockpool " + pool}))
    std::cout << "read time zone: " << timeZone << std::endl;
    TEST_ASSERT(clk->treatCommand(obd::core::Command{"clocktz " + timeZone}))
    std::cout << "read time zone: " << timeZone << std::endl;;
    TEST_ASSERT_EQUAL_STRING(pool.c_str(),clk->getPoolServer().c_str());
    TEST_ASSERT_EQUAL_STRING(timeZone.c_str(),clk->getTimeZone().c_str());
}

void test_config(){
    auto clk = baseSys.getDriver<obd::time::Clock>();
    auto hdd = baseSys.getDriver<obd::fs::FileSystem>();
    auto pool = clk->getPoolServer();
    auto timeZone = clk->getTimeZone();
    clk->saveConfigFile();
    TEST_ASSERT(hdd->exists(obd::fs::Path{"/config/Clock.cfg"}))
    clk->loadConfigFile();
    TEST_ASSERT_EQUAL_STRING(pool.c_str(),clk->getPoolServer().c_str());
    TEST_ASSERT_EQUAL_STRING(timeZone.c_str(),clk->getTimeZone().c_str());
}

void test_all() {
    UNITY_BEGIN();
    // tests one update
    RUN_TEST(test_bad_init);
    RUN_TEST(test_update);
    RUN_TEST(test_commands);
    RUN_TEST(test_config);
    UNITY_END();
}