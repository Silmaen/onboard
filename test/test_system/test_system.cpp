/**
 * @file test_system.cpp
 * @author argawaen
 * @date 11/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "../test_base.h"
#include "gfx/StatusLed.h"
#include "com/Console.h"
#include "fs/FileSystem.h"
#include "time/Clock.h"
#include "../DummyDriver.h"

void test_update(){
    baseSys.pushCommand(obd::core::Command{});
    auto timeStamp = baseSys.getTimestamp();
    baseSys.update();
    timeStamp = baseSys.getTimestamp()-timeStamp;
    TEST_ASSERT_LESS_OR_EQUAL_UINT64(10000,timeStamp);
}


void test_driver_management(){
    // try to add already present driver:
    TEST_ASSERT_FALSE(baseSys.addDriver<obd::gfx::StatusLed>())
    // try to delete driver non present
    TEST_ASSERT_FALSE(baseSys.deleteDriver<DummyDriver>())
    // add dummy driver
    TEST_ASSERT(baseSys.addDriver<DummyDriver>())
    // verify the auto init:
    TEST_ASSERT(baseSys.getDriver<DummyDriver>()->initialized())
    // test the driver delete
    TEST_ASSERT(baseSys.deleteDriver<DummyDriver>())
    TEST_ASSERT_EQUAL_STRING("StatusLed", baseSys.getDriver<obd::gfx::StatusLed>()->getName().c_str());
    TEST_ASSERT_EQUAL_STRING("Console", baseSys.getDriver<obd::com::Console>()->getName().c_str());
    TEST_ASSERT_EQUAL_STRING("FileSystem", baseSys.getDriver<obd::fs::FileSystem>()->getName().c_str());
    TEST_ASSERT_EQUAL_STRING("Clock", baseSys.getDriver<obd::time::Clock>()->getName().c_str());
}

void test_command(){
    obd::core::Command cmd(obd::com::Source::USB);
    TEST_ASSERT(cmd.empty())
    TEST_ASSERT(cmd.putChar('c'))
    TEST_ASSERT(cmd.putChar('d'))
    TEST_ASSERT_FALSE(cmd.empty())
    TEST_ASSERT(cmd.isCmd("cd"))
    TEST_ASSERT_FALSE(cmd.isCmd("cda"))
    TEST_ASSERT_EQUAL_STRING("", cmd.getParams().c_str());
    TEST_ASSERT(cmd.putChar(' '))
    TEST_ASSERT(cmd.putChar('a'))
    TEST_ASSERT_EQUAL_STRING("a", cmd.getParams().c_str());
}

void test_print(){
    baseSys.pushCommand(obd::core::Command{"help"});
    baseSys.pushCommand(obd::core::Command{"dmesg"});
    baseSys.pushCommand(obd::core::Command{"timeStat"});
    baseSys.pushCommand(obd::core::Command{"help bob"});
    baseSys.pushCommand(obd::core::Command{"help kernel"});
    baseSys.pushCommand(obd::core::Command{"help Clock"});
    baseSys.pushCommand(obd::core::Command{"date"});
    baseSys.pushCommand(obd::core::Command{"cfgSave"});
    baseSys.pushCommand(obd::core::Command{"cfgLoad"});
    baseSys.update();
    baseSys.update();
    baseSys.update();
    baseSys.update();
    baseSys.update();
    baseSys.update();
    baseSys.update();
    baseSys.update();
    baseSys.update();
    auto hdd = baseSys.getDriver<obd::fs::FileSystem>();
    TEST_ASSERT(hdd->exists(obd::fs::Path{"/config/Clock.cfg"}))
}

void test_driver(){
    auto drv = obd::core::BaseDriver(nullptr);
    drv.printInfo();
    drv.update(15);
    drv.printHelp();
    drv.loadConfigFile();
    drv.saveConfigFile();
    TEST_ASSERT_FALSE(drv.initialized());
}

void test_bad_systeminterface(){
    auto badSys = obd::core::SystemInterface(nullptr);
    TEST_ASSERT_NULL(badSys.getParentOutput());
    badSys.print(' ');
    badSys.print(" ");
    badSys.print(std::string());
    badSys.print(static_cast<uint8_t>(0));
    badSys.print(static_cast<int8_t>(0));
    badSys.print(static_cast<uint16_t>(0));
    badSys.print(static_cast<int16_t>(0));
    badSys.print(static_cast<uint32_t>(0));
    badSys.print(static_cast<int32_t>(0));
    badSys.print(static_cast<uint64_t>(0));
    badSys.print(static_cast<int64_t>(0));
    badSys.print(0.0);
    badSys.println(' ');
    badSys.println(" ");
    badSys.println(std::string());
    badSys.printBool(true);
    badSys.println(static_cast<uint8_t>(0));
    badSys.println(static_cast<int8_t>(0));
    badSys.println(static_cast<uint16_t>(0));
    badSys.println(static_cast<int16_t>(0));
    badSys.println(static_cast<uint32_t>(0));
    badSys.println(static_cast<int32_t>(0));
    badSys.println(static_cast<uint64_t>(0));
    badSys.println(static_cast<int64_t>(0));
    badSys.println(0.0);
    badSys.printlnBool(true);
    badSys.println();
    badSys.pushCommand(obd::core::Command());
}

void test_good_systeminterface(){
    auto badSys = obd::core::SystemInterface(&baseSys);
    TEST_ASSERT_NOT_NULL(badSys.getParentOutput());
    badSys.print(' ');
    badSys.print(" ");
    badSys.print(std::string());
    badSys.print(static_cast<uint8_t>(0));
    badSys.print(static_cast<int8_t>(0));
    badSys.print(static_cast<uint16_t>(0));
    badSys.print(static_cast<int16_t>(0));
    badSys.print(static_cast<uint32_t>(0));
    badSys.print(static_cast<int32_t>(0));
    badSys.print(static_cast<uint64_t>(0));
    badSys.print(static_cast<int64_t>(0));
    badSys.print(0.0);
    badSys.println(' ');
    badSys.println(" ");
    badSys.println(std::string());
    badSys.printBool(true);
    badSys.printBool(false);
    badSys.println(static_cast<uint8_t>(0));
    badSys.println(static_cast<int8_t>(0));
    badSys.println(static_cast<uint16_t>(0));
    badSys.println(static_cast<int16_t>(0));
    badSys.println(static_cast<uint32_t>(0));
    badSys.println(static_cast<int32_t>(0));
    badSys.println(static_cast<uint64_t>(0));
    badSys.println(static_cast<int64_t>(0));
    badSys.println(0.0);
    badSys.printlnBool(true);
    badSys.printlnBool(false);
    badSys.println();
    badSys.pushCommand(obd::core::Command());
}

void test_source(){
    TEST_ASSERT_EQUAL_STRING(" none   > '",obd::com::sourceToStrPrompt(obd::com::Source::NONE).c_str());
    TEST_ASSERT_EQUAL_STRING(" usb    > '",obd::com::sourceToStrPrompt(obd::com::Source::USB).c_str());
    TEST_ASSERT_EQUAL_STRING(" uart   > '",obd::com::sourceToStrPrompt(obd::com::Source::UART_0).c_str());
    TEST_ASSERT_EQUAL_STRING(" telnet > '",obd::com::sourceToStrPrompt(obd::com::Source::TELNET).c_str());
    TEST_ASSERT_EQUAL_STRING("",obd::com::sourceToStrPrompt(static_cast<obd::com::Source>(-1)).c_str());
}

void test_all() {
    UNITY_BEGIN();
    // tests one update
    RUN_TEST(test_update);
    RUN_TEST(test_driver_management);
    RUN_TEST(test_command);
    RUN_TEST(test_print);
    RUN_TEST(test_driver);
    RUN_TEST(test_bad_systeminterface);
    RUN_TEST(test_good_systeminterface);
    RUN_TEST(test_source);
    UNITY_END();
}
