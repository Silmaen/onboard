/**
 * @author Silmaen
 * @date 29/05/2021
 */

#include <obd_basedriver.h>
#include <obd_system.h>

namespace obd::core {

Print* baseDriver::getParentPrint() {
    if (parent == nullptr)
        return nullptr;
    return parent->getPrint();
}

size_t baseDriver::print(const __FlashStringHelper* s) {
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s);
}

size_t baseDriver::print(const String & s){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s);
}

size_t baseDriver::print(const char* s){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s);
}

size_t baseDriver::print(char s){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s);
}

size_t baseDriver::print(unsigned char s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t baseDriver::print(int16_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t baseDriver::print(uint16_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t baseDriver::print(int32_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t baseDriver::print(uint32_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t baseDriver::print(int64_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t baseDriver::print(uint64_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t baseDriver::print(double s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s, i);
}

size_t baseDriver::print(const Printable& s){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->print(s);
}


size_t baseDriver::println(const __FlashStringHelper *s){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s);
}

size_t baseDriver::println(const String &s){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s);
}

size_t baseDriver::println(const char* s){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s);
}

size_t baseDriver::println(char s){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s);
}

size_t baseDriver::println(unsigned char s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t baseDriver::println(int16_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t baseDriver::println(uint16_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t baseDriver::println(int32_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t baseDriver::println(uint32_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t baseDriver::println(int64_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t baseDriver::println(uint64_t s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t baseDriver::println(double s, int i){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s, i);
}

size_t baseDriver::println(const Printable& s){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println(s);
}

size_t baseDriver::println(){
    if (getParentPrint() == nullptr)
        return 0;
    return getParentPrint()->println();
}

void baseDriver::pushCommand(const command& cmd) const {
    if (getParent() != nullptr) {
        getParent()->pushCommand(cmd);
    }
}

std::shared_ptr<baseDriver> baseDriver::getDriver(const String& name) {
    if (getParent() != nullptr)
        return getParent()->getDriver(name);
    return nullptr;
}

}// namespace obd::core
