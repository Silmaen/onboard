/**
 * @file test_data.cpp
 * @author argawaen
 * @date 12/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"
#include "core/driver/Messenger.h"
#include "core/driver/Node.h"

using namespace obd::core::driver;

void test_creation() {
    Node drv(nullptr);
    TEST_ASSERT_EQUAL_STRING("Node", drv.name().c_str());
    TEST_ASSERT_FALSE(drv.initialized())
    TEST_ASSERT_FALSE(drv.pushMessage(Message{0, 1}))
    drv.update();
    drv.saveConfig();
    drv.loadConfig();
    TEST_ASSERT_EQUAL_STRING("", drv.info().c_str());
    drv.init();// should fail because no messenger
    TEST_ASSERT_FALSE(drv.initialized());
}

void test_initialization() {
    std::shared_ptr<Messenger> msg = std::make_shared<Messenger>(nullptr);
    Node drv(msg);
    drv.init();
    drv.update();
    TEST_ASSERT(drv.initialized())
    drv.terminate();
    TEST_ASSERT_FALSE(drv.initialized())
    TEST_ASSERT_EQUAL(Node::Category::None, drv.category());
}

void test_message() {
    std::shared_ptr<Messenger> msg = std::make_shared<Messenger>(nullptr);
    Node drv(msg);
    drv.init();
    // messages always accepted
    TEST_ASSERT(drv.pushMessage(Message{0, 1}))
    // bad destination
    TEST_ASSERT_FALSE(drv.pushMessage(Message{0, 1, "bob", Message::MessageType::Command}))
    // command unknown
    TEST_ASSERT_FALSE(drv.pushMessage(Message{0, drv.type(), "bob", Message::MessageType::Command}))
    // known command
    TEST_ASSERT(drv.pushMessage(Message{0, drv.type(), "info", Message::MessageType::Command}))
    // known command sent to all
    TEST_ASSERT(drv.pushMessage(Message{0, 0, "info", Message::MessageType::Command}))
    // known command but destination unknown
    TEST_ASSERT_FALSE(drv.pushMessage(Message{0, 185, "info", Message::MessageType::Command}))
}

void test_treatMessages() {
    std::shared_ptr<Messenger> msg = std::make_shared<Messenger>(nullptr);
    Node drv(msg);
    drv.init();
    TEST_ASSERT(drv.pushMessage(Message{0, drv.type(), "info", Message::MessageType::Command}))
    TEST_ASSERT(drv.pushMessage(Message{0, drv.type(), "hello!", Message::MessageType::Message}))
    TEST_ASSERT(drv.pushMessage(Message{0, drv.type(), "info", Message::MessageType::Reply}))
    TEST_ASSERT(drv.pushMessage(Message{0, drv.type(), "info", Message::MessageType::Command}))
    TEST_ASSERT(drv.pushMessage(Message{0, drv.type(), "info", Message::MessageType::Message}))
    TEST_ASSERT(drv.pushMessage(Message{0, drv.type(), "info", Message::MessageType::Command}))
    TEST_ASSERT(drv.pushMessage(Message{0, drv.type(), "info", Message::MessageType::Command}))
    TEST_ASSERT_FALSE(drv.pushMessage(Message{0, drv.type(), "boby", Message::MessageType::Command}))
    TEST_ASSERT_EQUAL(7, drv.queueSize());
    drv.update();
    TEST_ASSERT_EQUAL(1, drv.queueSize());
    drv.update();
    TEST_ASSERT_EQUAL(0, drv.queueSize());
}

void test_all() {
    UNITY_BEGIN();
    RUN_TEST(test_creation);
    RUN_TEST(test_initialization);
    RUN_TEST(test_message);
    RUN_TEST(test_treatMessages);
    UNITY_END();
}
