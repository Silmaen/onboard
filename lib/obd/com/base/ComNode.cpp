/**
 * @file ComNode.cpp
 * @author argawaen
 * @date 19/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "ComNode.h"

namespace obd::com::base {

void ComNode::preTreatment() {
    Node::preTreatment();
    listen();
    talk();
}

void ComNode::postTreatment() {
    Node::postTreatment();
}

void ComNode::listen() {
    if (available()){
        OString msg = readLine();
        console(msg,MessageType::Input);
    }
}

void ComNode::talk() {

}

bool ComNode::treatMessage(const core::driver::Message& message) {
    if (Node::treatMessage(message)){
        return true;
    }
    if (message.isMessage()){
        OString affichage;
        affichage += computeName(message.getSource()) + " > ";
        if (message.getType() == MessageType::Error){
            affichage += "ERROR ";
        }
        if (message.getType() == MessageType::Warning){
            affichage += "WARNING ";
        }
        affichage += message.getMessage();
        writeLine(affichage);
        return true;
    }
    return false;
}

}// namespace obd::com::base
