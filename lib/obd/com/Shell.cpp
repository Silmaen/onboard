/**
 * @file Shell.cpp
 * @author argawaen
 * @date 19/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Shell.h"
#include "core/driver/Messenger.h"
#ifdef ESP8266
#include <core_esp8266_features.h>
#endif
namespace obd::com {

void Shell::addOutput(size_t hcd) {
    outputs.insert(hcd);
}

void Shell::removeOutput(size_t hcd) {
    outputs.erase(hcd);
}

bool Shell::treatMessage(const Message& message) {
    if (Node::treatMessage(message)) {
        return true;
    }
    if (message.isMessage()){
        outputMessage(message);
        return true;
    }
    if (message.getType() == MessageType::Input) {
        outputMessage(message.getMessage(), MessageType::Message);// echo message
        // check for a shell command
        if (shellCommand(message))
            return true;
        size_t nodeId = getMessenger()->computeId(message.getBaseCommand());
        if (nodeId != 0) {
            if (message.hasParams()) {
                broadcastMessage(nodeId, message.getParamStr(), MessageType::Command);
            }else{
                outputMessage(F("Driver name alone need a command."), MessageType::Error);
            }
            return true;
        }
    }
    outputMessage(Message{type(),0,F("Unknown command"), MessageType::Error});// echo message
    return false;
}

void Shell::outputMessage(const core::driver::Message& message) {
    for(const auto& output : outputs){
        broadcastMessage(Message{message.getSource(), output, message.getMessage(), message.getType()});
    }
}

void Shell::outputMessage(const OString& message, const core::driver::Node::MessageType& messageType) {
    outputMessage(Message{type(),0,message, messageType});
}

bool Shell::shellCommand([[maybe_unused]]const core::driver::Message& message) {
    if (message.getBaseCommand() == F("dmesg")){
        dmesg();
        return true;
    }
    if (message.getBaseCommand() == F("lsdrv")){
        lsdrv();
        return true;
    }
    return false;
}

void Shell::dmesg() {
#ifdef ARDUINO
#ifdef ESP8266
    outputMessage(F("System ESP8266"), MessageType::Message);

#endif
#else
    output
#endif
}

void Shell::lsdrv() {
    auto list = getMessenger()->getDriverList();
    outputMessage(F("List of drivers"), MessageType::Message);
    for(auto driver:list){
        outputMessage(driver, MessageType::Message);
    }
}

}// namespace obd::com
