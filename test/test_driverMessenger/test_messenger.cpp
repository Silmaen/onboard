/**
 * @file test_messenger.cpp
 * @author argawaen
 * @date 12/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"
#include "core/driver/Manager.h"
#include "core/driver/Messenger.h"
#include "gfx/StatusLed.h"

using namespace obd::core::driver;

void test_void() {
    Messenger msg(nullptr);
    msg.pushMessage(Message{0, 1, "yo"});
    msg.update();
    msg.check();
    TEST_ASSERT_EQUAL(0, msg.size());
    TEST_ASSERT_EQUAL(1, msg.stats().droppedMessage);
}

void test_managed() {
    std::shared_ptr<Manager> mng   = std::make_shared<Manager>();
    std::shared_ptr<Messenger> msg = std::make_shared<Messenger>(mng);
    std::shared_ptr<Node> node     = std::make_shared<Node>(msg);
    mng->addNode(node);
    node->init();
    TEST_ASSERT(node->initialized());
    // push several messages
    msg->pushMessage(Message{0, 1, "yo"});
    msg->pushMessage(Message{0, node->type(), "yo", Message::MessageType::Command});
    msg->pushMessage(Message{0, node->type(), "yo", Message::MessageType::Command});
    msg->pushMessage(Message{0, node->type(), "info", Message::MessageType::Command});
    msg->pushMessage(Message{0, node->type(), "yo1"});
    msg->pushMessage(Message{0, node->type(), "yo2"});
    msg->pushMessage(Message{0, node->type(), "yo3"});
    msg->pushMessage(Message{0, node->type(), "yo4"});
    msg->pushMessage(Message{0, 0, "yo2", Message::MessageType::Command});
    msg->pushMessage(Message{0, 0, "yo3"});
    msg->pushMessage(Message{0, 0, "yo3"});
    msg->pushMessage(Message{0, 0, "yo3"});
    msg->pushMessage(Message{0, 0, "yo3"});
    msg->pushMessage(Message{0, 0, "yo3"});
    msg->pushMessage(Message{0, 0, "yo3"});
    TEST_ASSERT_EQUAL(15, msg->size());
    TEST_ASSERT_EQUAL(15, msg->stats().receivedMessages);
    msg->update();
    TEST_ASSERT_EQUAL(10, node->messageSize());
    node->update();
    TEST_ASSERT_EQUAL(5, msg->size());
    TEST_ASSERT_EQUAL(19, msg->stats().receivedMessages);
    TEST_ASSERT_EQUAL(10, msg->stats().sentMessages);
    TEST_ASSERT_EQUAL(4, msg->stats().droppedMessage);
}

void test_name(){
    auto msger = baseSys.getMessenger();
    TEST_ASSERT_NOT_NULL(msger)
    TEST_ASSERT_EQUAL_STRING("All", msger->computeName(0).c_str());
    TEST_ASSERT_EQUAL_STRING("Unknown", msger->computeName(111).c_str());
    auto node = baseSys.getNode<obd::gfx::StatusLed>();
    TEST_ASSERT_NOT_NULL(msger)
    TEST_ASSERT_EQUAL_STRING("StatusLed", msger->computeName(node->type()).c_str());
}

void test_all() {
    UNITY_BEGIN();
    RUN_TEST(test_void);
    RUN_TEST(test_managed);
    RUN_TEST(test_name);
    UNITY_END();
}
