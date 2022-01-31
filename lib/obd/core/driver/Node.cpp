/**
 * @file Node.cpp
 * @author argawaen
 * @date 14/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Node.h"
#include "Messenger.h"
#include "com/Shell.h"
#include <algorithm>
#include <utility>

namespace obd::core::driver {

Node::Node(std::shared_ptr<Messenger> messenger) :
    messenger{std::move(messenger)} {
}

void Node::init() {
    if (messenger == nullptr) {
        // cannot initialize without messenger
        _initialized = false;
        return;
    }
    Object::init();
}

void Node::update() {
    Object::update();
    if (!initialized())
        return;
    preTreatment();
    uint8_t messageCount = 0;
    while (!messages.empty()) {
        if (treatMessage(messages.front())) {
            ++messageCount;
        }
        // DROP unsent messages
        messages.pop();
        // limit the number of sent message per frame
        if (messageCount >= maxFrameMessages)
            break;
    }
    postTreatment();
}

void Node::loadConfig() {
    // by default, do nothing
}

void Node::saveConfig() const {
    // by default, do nothing
}

void Node::preTreatment() {
    // by default, do nothing
}

void Node::postTreatment() {
    // by default, do nothing
}

bool Node::pushMessage(const Message& message) {
    if (!initialized()) {
        return false;
    }
    if (message.getType() != MessageType::Command) {
        messages.push(message);
        return true;
    }
    if (!message.isForMe(type())) {
        console("Bad Destination", MessageType::Warning);
        return false;
    }
    if (pushCommand(message)) {
        return true;
    }
    console("Unknown Command", MessageType::Warning);
    return false;
}

bool Node::pushCommand(const Message& message) {
    if (message.getBaseCommand() == "info") {
        messages.push(message);
        return true;
    }
    // Reject all other commands
    return false;
}

OString Node::info() const {
    // by default, do nothing
    return {};
}

void Node::broadcastMessage(const Message& message) const {
    messenger->pushMessage(message);
}

bool Node::treatMessage(const Message& message) {
    if (message.getType() == MessageType::Command) {
        if (message.getBaseCommand() == "info") {
            broadcastMessage(message.getSource(), info(), MessageType::Reply);
            return true;
        }
    }
    return false;
}

bool Node::linkNode([[maybe_unused]] const std::shared_ptr<Node>& node) {
    return false;
}

void Node::broadcastMessage(const size_t& destination, const OString& msg, MessageType mtype) const {
    broadcastMessage(Message{type(), destination, msg, mtype});
}

size_t Node::getConsoleId() {
    return typeid(com::Shell).hash_code();
}

void Node::console(const OString& msg, MessageType type) const {
    broadcastMessage(getConsoleId(), msg, type);
}

OString Node::computeName(const size_t nodeId) {
    return messenger->computeName(nodeId);
}

}// namespace obd::core::driver
