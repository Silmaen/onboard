/**
 * @file test_system.cpp
 * @author argawaen
 * @date 17/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "../test_base.h"
#include "core/System.h"

using namespace obd::core;

void test_creation(){
    System sys;
    TEST_ASSERT_FALSE(sys.check());
    sys.update();
    sys.init();
    TEST_ASSERT(sys.check());
}

class MyNode:public driver::Node{
public:
    explicit MyNode(const std::shared_ptr<driver::Messenger>& msg):Node{msg}{}
    bool linkNode(const std::shared_ptr<Node>& node) override{
        if (node->type() != code<Node>())
            return false;
        nodeLink = node;
        return true;
    }
private:
    std::shared_ptr<Node> nodeLink = nullptr;
};

void test_addNode(){
    System sys;
    TEST_ASSERT(sys.addNode<driver::Node>())
    sys.init();
    TEST_ASSERT(sys.check());
    TEST_ASSERT_FALSE(sys.addNode<driver::Node>())
    TEST_ASSERT_FALSE(sys.linkNodes(0,0))
    TEST_ASSERT_FALSE(sys.linkNodes(sys.code<driver::Node>(),0))
    TEST_ASSERT_FALSE(sys.linkNodes(sys.code<driver::Node>(),sys.code<driver::Node>()))
    TEST_ASSERT(sys.addNode<MyNode>())
    TEST_ASSERT_FALSE(sys.linkNodes(sys.code<driver::Node>(),sys.code<MyNode>()))
    TEST_ASSERT(sys.linkNodes(sys.code<MyNode>(), sys.code<driver::Node>()))
    sys.update();
}

void test_all() {
    UNITY_BEGIN();
    // tests one update
    RUN_TEST(test_creation);
    RUN_TEST(test_addNode);
    UNITY_END();
}
