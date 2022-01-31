/**
 * @file test_messenger.cpp
 * @author argawaen
 * @date 12/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"
#include "core/driver/Manager.h"
#include <iostream>
#include <utility>


using namespace obd::core::driver;

void test_getNode() {
    Manager mng;
    mng.update();
    mng.addNode(std::make_shared<Node>(nullptr));
    auto hash = mng.begin()->get()->type();
    mng.addNode(std::make_shared<Node>(nullptr));// second times should fail
    size_t counter = 0;
    for (auto node = mng.cbegin(); node != mng.cend(); ++node) {
        TEST_ASSERT_EQUAL_STRING("Node", (*node)->name().c_str());
        ++counter;
    }
    TEST_ASSERT_EQUAL(1, counter);
    TEST_ASSERT_NOT_NULL(mng.getNode(hash))
    TEST_ASSERT_NOT_NULL(mng.getNode("Node"))
    TEST_ASSERT_NULL(mng.getNode("toto"))
    TEST_ASSERT_NULL(mng.getNode(0))
}

void test_getDriver() {
    Manager mng;
    mng.addNode(std::make_shared<Node>(nullptr));
    class OtherNode : public Node {
    public:
        explicit OtherNode(std::shared_ptr<Messenger>&& msg) :
            Node(std::move(msg)) {}
    };
    TEST_ASSERT_EQUAL(1, mng.size());
    TEST_ASSERT_NOT_NULL(mng.getDriver<Node>())
    TEST_ASSERT_NULL(mng.getDriver<OtherNode>())
    TEST_ASSERT_NULL(mng.getDriver<Manager>())
    mng.addNode(std::make_shared<OtherNode>(nullptr));
    TEST_ASSERT_NOT_NULL(mng.getDriver<OtherNode>())
}

void test_all() {
    UNITY_BEGIN();
    RUN_TEST(test_getNode);
    RUN_TEST(test_getDriver);
    UNITY_END();
}
