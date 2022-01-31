/**
 * @file System.cpp
 * @author argawaen
 * @date 14/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "System.h"
#include "gfx/StatusLed.h"
//#include "fs/FileSystem.h"
//#include "time/Clock.h"
#include "com/Shell.h"
#include "com/Stdout.h"

namespace obd::core {

System::System() :
    manager{std::make_shared<driver::Manager>()},
    messenger{std::make_shared<driver::Messenger>(manager)}{}

System::~System() = default;

void System::init() {
    Object::init();
    messenger->init();
    addNode<gfx::StatusLed>();
    addNode<com::Shell>();

    addNode<com::Stdout>();
    manager->getDriver<com::Shell>()->addOutput(code<com::Stdout>());


    //    addNode<fs::FileSystem>();
    //    addNode<time::Clock>();
    //    linkNodes(code<obd::time::Clock>(), code<obd::fs::FileSystem>());
    //    linkNodes(code<obd::fs::FileSystem>(), code<obd::time::Clock>());

    manager->init();
}

void System::update() {
    if (!initialized()) {
        return;
    }
    messenger->update();
    manager->update();
}

bool System::check() {
    if (!initialized())
        return false;
    return true;
}

bool System::linkNodes(const size_t& node, const size_t& link) {
    auto n_node = manager->getNode(node);
    if (n_node == nullptr)
        return false;
    auto n_link = manager->getNode(link);
    if (n_link == nullptr)
        return false;
    return n_node->linkNode(n_link);
}

}// namespace obd::core
