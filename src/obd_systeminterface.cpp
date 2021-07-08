/**
 * \author Silmaen
 * \date 06/07/2021
 */

#include <obd_systeminterface.h>
#include <obd_system.h>

namespace obd::core {

Print* SystemInterface::getParentPrint() const{
    if (parent == nullptr)
        return nullptr;
    return parent->getPrint();
}

size_t SystemInterface::print(const __FlashStringHelper* s) const {
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s);
}

size_t SystemInterface::print(const String & s) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s);
}

size_t SystemInterface::print(const char* s) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s);
}

size_t SystemInterface::print(char s) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s);
}

size_t SystemInterface::print(unsigned char s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t SystemInterface::print(int16_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t SystemInterface::print(uint16_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t SystemInterface::print(int32_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t SystemInterface::print(uint32_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t SystemInterface::print(int64_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t SystemInterface::print(uint64_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t SystemInterface::print(double s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t SystemInterface::print(const Printable& s) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s);
}


size_t SystemInterface::println(const __FlashStringHelper *s) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s);
}

size_t SystemInterface::println(const String &s) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s);
}

size_t SystemInterface::println(const char* s) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s);
}

size_t SystemInterface::println(char s) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s);
}

size_t SystemInterface::println(unsigned char s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t SystemInterface::println(int16_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t SystemInterface::println(uint16_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t SystemInterface::println(int32_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t SystemInterface::println(uint32_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t SystemInterface::println(int64_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t SystemInterface::println(uint64_t s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t SystemInterface::println(double s, int i) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t SystemInterface::println(const Printable& s) const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s);
}

size_t SystemInterface::println() const{
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println();
}

void SystemInterface::pushCommand(const command& cmd) const {
    if (getParent() != nullptr) {
        getParent()->pushCommand(cmd);
    }
}

}// namespace obd::core
