/**
 * \author Silmaen
 * \date 06/07/2021
 */

#include <obd_system.h>
#include <obd_systeminterface.h>

namespace obd::core {

Print* SystemInterface::getParentPrint() const {
    if (parent == nullptr)
        return nullptr;
    return parent->getPrint();
}

void SystemInterface::print(const __FlashStringHelper* s) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s);
}

void SystemInterface::print(const String& s) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s);
}

void SystemInterface::print(const char* s) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s);
}

void SystemInterface::print(char s) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s);
}

void SystemInterface::print(unsigned char s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s, i);
}

void SystemInterface::print(int16_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s, i);
}

void SystemInterface::print(uint16_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s, i);
}

void SystemInterface::print(int32_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s, i);
}

void SystemInterface::print(uint32_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s, i);
}

void SystemInterface::print(int64_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s, i);
}

void SystemInterface::print(uint64_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s, i);
}

void SystemInterface::print(double s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s, i);
}

void SystemInterface::print(const Printable& s) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->print(s);
}


void SystemInterface::println(const __FlashStringHelper* s) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s);
}

void SystemInterface::println(const String& s) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s);
}

void SystemInterface::println(const char* s) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s);
}

void SystemInterface::println(char s) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s);
}

void SystemInterface::println(unsigned char s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s, i);
}

void SystemInterface::println(int16_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s, i);
}

void SystemInterface::println(uint16_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s, i);
}

void SystemInterface::println(int32_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s, i);
}

void SystemInterface::println(uint32_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s, i);
}

void SystemInterface::println(int64_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s, i);
}

void SystemInterface::println(uint64_t s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s, i);
}

void SystemInterface::println(double s, int i) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s, i);
}

void SystemInterface::println(const Printable& s) const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(s);
}

void SystemInterface::println() const {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println();
}

void SystemInterface::pushCommand(const command& cmd) const {
    if (getParent() != nullptr) {
        getParent()->pushCommand(cmd);
    }
}

void SystemInterface::printBool(bool ptr) const {
    if (getParentPrint() == nullptr)
        return;
    if (ptr) {
        print(F("true"));
    } else {
        print(F("false"));
    }
}

void SystemInterface::printlnBool(bool ptr) const {
    if (getParentPrint() == nullptr)
        return;
    if (ptr) {
        println(F("true"));
    } else {
        println(F("false"));
    }
}

}// namespace obd::core
