/**
 * @file SystemInterface.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "SystemInterface.h"
#include "System.h"


namespace obd::core {

com::Output* SystemInterface::getParentOutput() const {
    if (parent == nullptr)
        return nullptr;
    return parent->getOutput();
}

#ifdef ARDUINO
void SystemInterface::print(const __FlashStringHelper* s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s);
}

void SystemInterface::print(const String& s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s);
}
#endif


void SystemInterface::print(const std::string& s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s);
}

void SystemInterface::print(const char* s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s);
}

void SystemInterface::print(char s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s);
}

void SystemInterface::print(unsigned char s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s, format);
}

void SystemInterface::print(int16_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s, format);
}

void SystemInterface::print(uint16_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s, format);
}

void SystemInterface::print(int32_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s, format);
}

void SystemInterface::print(uint32_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s, format);
}

void SystemInterface::print(int64_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s, format);
}

void SystemInterface::print(uint64_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s, format);
}

void SystemInterface::print(double s, int i) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s, i);
}
#ifdef ARDUINO
void SystemInterface::print(const Printable& s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->print(s);
}


void SystemInterface::println(const __FlashStringHelper* s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s);
}

void SystemInterface::println(const String& s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s);
}
#endif

void SystemInterface::println(const std::string& s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s);
}

void SystemInterface::println(const char* s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s);
}

void SystemInterface::println(char s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s);
}

void SystemInterface::println(unsigned char s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s, format);
}

void SystemInterface::println(int16_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s, format);
}

void SystemInterface::println(uint16_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s, format);
}

void SystemInterface::println(int32_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s, format);
}

void SystemInterface::println(uint32_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s, format);
}

void SystemInterface::println(int64_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s, format);
}

void SystemInterface::println(uint64_t s, com::Format format) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s, format);
}

void SystemInterface::println(double s, int i) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s, i);
}

#ifdef ARDUINO
void SystemInterface::println(const Printable& s) const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println(s);
}
#endif

void SystemInterface::println() const {
    if (getParentOutput() == nullptr)
        return;
    getParentOutput()->println();
}

void SystemInterface::pushCommand(const Command& cmd) const {
    if (getParent() != nullptr) {
        getParent()->pushCommand(cmd);
    }
}

void SystemInterface::printBool(bool ptr) const {
    if (getParentOutput() == nullptr)
        return;
    if (ptr) {
        print(F("true"));
    } else {
        print(F("false"));
    }
}

void SystemInterface::printlnBool(bool ptr) const {
    if (getParentOutput() == nullptr)
        return;
    if (ptr) {
        println(F("true"));
    } else {
        println(F("false"));
    }
}

}// namespace obd::core

