/**
 * @file Manager.cpp
 * @author argawaen
 * @date 14/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Manager.h"

namespace obd::core::driver {

Manager::NodeType Manager::getNode(const Manager::NameType& nodeName)  {
    auto search = find_if(begin(),end(),[nodeName](const NodeType& driver) { return nodeName == driver->name(); });
    if (search == end())
        return nullptr;
    return *search;
}

Manager::NodeType Manager::getNode(const size_t& nodeHash) {
    auto search = find_if(begin(),end(),[nodeHash](const NodeType& driver) { return nodeHash == driver->type(); });
    if (search == end())
        return nullptr;
    return *search;
}

bool Manager::addNode(const NodeType& node) {;
    auto search = find_if(begin(),end(),[node](const NodeType& driver) { return node->type() == driver->type(); });
    if (search == end()) {
        nodes.push_back(node);
        return true;
    }
    return false;
}
void Manager::init() {
    Object::init();
    for(const auto& node:nodes){
        node->init();
    }
}
void Manager::update() {
    if (!initialized()) {
        return;
    }
    for(const auto& node:nodes){
        node->update();
    }
}

}// namespace obd::core::driver
