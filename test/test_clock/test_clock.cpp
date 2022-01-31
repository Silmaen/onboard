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
    badClock.init();
    TEST_ASSERT_FALSE(badClock.initialized())
    auto messenger = std::make_shared<obd::core::driver::Messenger>(nullptr);
    auto badClock2 = obd::time::Clock(messenger);
    badClock2.init();
    TEST_ASSERT(badClock2.initialized())
    auto hdd = std::make_shared<obd::fs::FileSystem>(messenger);
    badClock2.linkNode(hdd);
    badClock2.init();
    TEST_ASSERT(badClock2.initialized())
}

void test_update(){
    auto clk =  baseSys.getNode<obd::time::Clock>();
    auto hdd = baseSys.getNode<obd::fs::FileSystem>();
    obd::fs::Path time_save{obd::config::tsSave};
    TEST_ASSERT(hdd->mkdir(time_save.parent(),true,true))
    clk->accelerateTime(obd::config::saveInterval + 1);// try to provoque a time save
    clk->update();
    TEST_ASSERT(hdd->exists(obd::fs::Path{obd::config::tsSave}))
}

#include <iostream>
void test_commands(){
    auto clk = baseSys.getNode<obd::time::Clock>();

}

void test_config(){
    auto clk = baseSys.getNode<obd::time::Clock>();
    auto hdd = baseSys.getNode<obd::fs::FileSystem>();
    auto pool = clk->getPoolServer();
    auto timeZone = clk->getTimeZone();
    clk->saveConfig();
    TEST_ASSERT(hdd->exists(obd::fs::Path{"/config/Clock.cfg"}))
    clk->loadConfig();
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