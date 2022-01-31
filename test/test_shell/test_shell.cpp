/**
 * @file test_system.cpp
 * @author argawaen
 * @date 17/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "../test_base.h"
#include "com/Shell.h"

#include <iostream>
#include <sstream>

void test_command(){
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ostringstream strCout;
    std::cout.rdbuf( strCout.rdbuf() );

    auto shell = baseSys.getNode<obd::com::Shell>();
    TEST_ASSERT_EQUAL(obd::core::driver::Category::Console, shell->category());
    TEST_ASSERT(shell->pushMessage(obd::core::driver::Message{0,shell->type(),"hello world!",obd::core::driver::Message::MessageType::Message}));
    baseSys.update();
    baseSys.update();
    TEST_ASSERT_EQUAL_STRING("All > hello world!\n", strCout.str().c_str());
    strCout.str("");
    TEST_ASSERT(shell->pushMessage(obd::core::driver::Message{12,shell->type(),"hello world!",obd::core::driver::Message::MessageType::Warning}));
    baseSys.update();
    baseSys.update();
    TEST_ASSERT_EQUAL_STRING("Unknown > WARNING hello world!\n", strCout.str().c_str());
    strCout.str("");
    TEST_ASSERT(shell->pushMessage(obd::core::driver::Message{0,shell->type(),"hello world!",obd::core::driver::Message::MessageType::Error}));
    baseSys.update();
    baseSys.update();
    TEST_ASSERT_EQUAL_STRING("All > ERROR hello world!\n", strCout.str().c_str());
    strCout.str("");
    TEST_ASSERT(shell->pushMessage(obd::core::driver::Message{0,shell->type(),"hello world!",obd::core::driver::Message::MessageType::Reply}));
    baseSys.update();
    baseSys.update();
    TEST_ASSERT_EQUAL_STRING("", strCout.str().c_str());
    strCout.str("");

    // Restore old cout.
    std::cout.rdbuf( oldCoutStreamBuf );
}

void test_all() {
    UNITY_BEGIN();
    // tests one update
    RUN_TEST(test_command);
    UNITY_END();
}
