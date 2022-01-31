/**
 * @file SystemInterface.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "SystemInterface.h"
#include "System_old.h"

namespace obd::core {

/// simple void output
static com::Output dummy;

com::Output& SystemInterface::getParentOutput() const {
    if (parent != nullptr) {
        return parent->getOutput();
    }
    return dummy;
}

void SystemInterface::print(const OString& data) const {
    getParentOutput().print(data);
}

void SystemInterface::print(const char* data) const {
    getParentOutput().print(data);
}

void SystemInterface::print(char data) const {
    getParentOutput().print(data);
}

void SystemInterface::print(unsigned char data, com::Format format) const {
    getParentOutput().print(data, format);
}

void SystemInterface::print(int16_t data, com::Format format) const {
    getParentOutput().print(data, format);
}

void SystemInterface::print(uint16_t data, com::Format format) const {
    getParentOutput().print(data, format);
}

void SystemInterface::print(int32_t data, com::Format format) const {
    getParentOutput().print(data, format);
}

void SystemInterface::print(uint32_t data, com::Format format) const {
    getParentOutput().print(data, format);
}

void SystemInterface::print(int64_t data, com::Format format) const {
    getParentOutput().print(data, format);
}

void SystemInterface::print(uint64_t data, com::Format format) const {
    getParentOutput().print(data, format);
}

void SystemInterface::print(double data, int digits) const {
    getParentOutput().print(data, digits);
}

void SystemInterface::println(const OString& data) const {
    getParentOutput().println(data);
}

void SystemInterface::println(const char* data) const {
    getParentOutput().println(data);
}

void SystemInterface::println(char data) const {
    getParentOutput().println(data);
}

void SystemInterface::println(unsigned char data, com::Format format) const {
    getParentOutput().println(data, format);
}

void SystemInterface::println(int16_t data, com::Format format) const {
    getParentOutput().println(data, format);
}

void SystemInterface::println(uint16_t data, com::Format format) const {
    getParentOutput().println(data, format);
}

void SystemInterface::println(int32_t data, com::Format format) const {
    getParentOutput().println(data, format);
}

void SystemInterface::println(uint32_t data, com::Format format) const {
    getParentOutput().println(data, format);
}

void SystemInterface::println(int64_t data, com::Format format) const {
    getParentOutput().println(data, format);
}

void SystemInterface::println(uint64_t data, com::Format format) const {
    getParentOutput().println(data, format);
}

void SystemInterface::println(double data, int digit) const {
    getParentOutput().println(data, digit);
}

void SystemInterface::println() const {
    getParentOutput().println();
}

void SystemInterface::pushCommand(const Command& cmd) const {
    if (getParent() != nullptr) {
        parent->pushCommand(cmd);
    }
}

void SystemInterface::printBool(bool ptr) const {
    if (ptr) {
        print(F("true"));
    } else {
        print(F("false"));
    }
}

void SystemInterface::printlnBool(bool ptr) const {
    if (ptr) {
        println(F("true"));
    } else {
        println(F("false"));
    }
}

bool SystemInterface::addOutput(const std::shared_ptr<com::Output>& print) {
    if (parent==nullptr)
        return false;
    parent->addOutput(print);
    return true;
}

uint64_t SystemInterface::getTimestamp()const {
    return parent->getTimestamp();
}

}// namespace obd::core
