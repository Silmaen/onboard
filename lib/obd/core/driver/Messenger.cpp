/**
 * @file Messenger.cpp
 * @author argawaen
 * @date 14/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Messenger.h"

#include "Manager.h"
#include <utility>

namespace obd::core::driver {

Messenger::Messenger(std::shared_ptr<Manager> manager) :
    manager{std::move(manager)} {
}

void Messenger::pushMessage(const Message& message) {
    messages.push(message);
    ++statistics.receivedMessages;
}

void Messenger::update() {
    uint8_t messageCount = 0;
    while (!messages.empty()) {
        if (sendMessage(messages.front())) {
            ++messageCount;
            ++statistics.sentMessages;
        } else {
            ++statistics.droppedMessage;
        }
        // DROP unsent messages
        messages.pop();
        // limit the number of sent message per frame
        if (messageCount >= maxFrameMessages)
            break;
    }
}

bool Messenger::sendMessage(const Message& message) {
    if (manager == nullptr)
        return false;
    bool sent = false;
    for (auto& node : *manager) {
        if (message.isForMe(node->type())) {
            if (node->pushMessage(message))
                sent = true;
        }
    }
    return sent;
}

void Messenger::init() {
    Object::init();
}

bool Messenger::check() {
    if (manager == nullptr)
        return false;
    return Object::check();
}

OString Messenger::computeName(const size_t nodeId) {
    if (nodeId == 0)
        return "All";
    auto node = manager->getNode(nodeId);
    if (node == nullptr)
        return "Unknown";
    return node->name();
}
size_t Messenger::computeId(const OString& name) {
    if (name == "All")
        return 0;
    auto node = manager->getNode(name);
    if (node == nullptr)
        return 0;
    return node->type();
}

std::vector<OString> Messenger::getDriverList() const {
    std::vector<OString> nodes;
    for (auto& node : *manager)
        nodes.push_back(node->name());
    return nodes;
}

}// namespace obd::core::driver
