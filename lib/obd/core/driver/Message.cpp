/**
 * @file Message.cpp
 * @author argawaen
 * @date 14/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Message.h"
#include "data/DataUtils.h"
#include <bitset>
#include <iomanip>
#include <sstream>

namespace obd::core::driver {

Message::DataType Message::getBaseCommand() const {
    Message::DataType result = message.substr(0, message.find(' '));
    return result;
}

bool Message::hasParams() const {
    return message.find(' ') != OString::npos;
}

Message::MultipleData Message::getParams() const {
    if (message.find(' ') == OString::npos)
        return {};
    return data::split(getParamStr(), " ");
}
Message::DataType Message::getParamStr() const {
    return message.substr(message.find(' ') + 1);
}

void Message::print(const OString& data) {
    message += data;
}
void Message::print(const char* data) {
    print(OString(data));
}

void Message::print(int8_t data, Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
        oss << data;
        break;
    case Format::Decimal:
        oss << static_cast<int16_t>(data);
        break;
    case Format::Hexadecimal:
        oss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int16_t>(data);
        break;
    case Format::Binary:
        std::bitset<8> bitset(data);
        oss << bitset;
        break;
    }
    print(oss.str().c_str());
}

void Message::print(uint8_t data, Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
        oss << data;
        break;
    case Format::Decimal:
        oss << static_cast<uint16_t>(data);
        break;
    case Format::Hexadecimal:
        oss << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(data);
        break;
    case Format::Binary:
        std::bitset<8> bitset(data);
        oss << bitset;
        break;
    }
    print(oss.str().c_str());
}

void Message::print(int16_t data, Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        break;
    case Format::Hexadecimal:
        oss << std::setfill('0') << std::setw(4) << std::hex << data;
        break;
    case Format::Binary:
        std::bitset<16> bitset(data);
        oss << bitset;
        break;
    }
    print(oss.str().c_str());
}
void Message::print(uint16_t data, Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        break;
    case Format::Hexadecimal:
        oss << std::setfill('0') << std::setw(4) << std::hex << data;
        break;
    case Format::Binary:
        std::bitset<16> bitset(data);
        oss << bitset;
        break;
    }
    print(oss.str().c_str());
}
void Message::print(int32_t data, Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str().c_str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<int16_t>((data >> 16U) & 0xffff), format);
        print(static_cast<int16_t>(data & 0xffff), format);
        break;
    }
}
void Message::print(uint32_t data, Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str().c_str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<uint16_t>((data >> 16U) & 0xffff), format);
        print(static_cast<uint16_t>(data & 0xffff), format);
        break;
    }
}
void Message::print(int64_t data, Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str().c_str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<int16_t>((data >> 48U) & 0xffff), format);
        print(static_cast<int16_t>((data >> 32U) & 0xffff), format);
        print(static_cast<int16_t>((data >> 16U) & 0xffff), format);
        print(static_cast<int16_t>(data & 0xffff), format);
        break;
    }
}
void Message::print(uint64_t data, Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str().c_str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<uint16_t>((data >> 48U) & 0xffff), format);
        print(static_cast<uint16_t>((data >> 32U) & 0xffff), format);
        print(static_cast<uint16_t>((data >> 16U) & 0xffff), format);
        print(static_cast<uint16_t>(data & 0xffff), format);
        break;
    }
}
void Message::print(double data, int digit) {
    std::stringstream oss;
    oss << std::fixed << std::setprecision(digit) << data;
    print(oss.str().c_str());
}

void Message::println() {
    print("\n");
}

void Message::println(const OString& data) {
    print(data + "\n");
}

void Message::println(const char* data) {
    print(data);
    println();
}
void Message::println(int8_t data, Format format) {
    print(data, format);
    println();
}
void Message::println(uint8_t data, Format format) {
    print(data, format);
    println();
}
void Message::println(int16_t data, Format format) {
    print(data, format);
    println();
}
void Message::println(uint16_t data, Format format) {
    print(data, format);
    println();
}
void Message::println(int32_t data, Format format) {
    print(data, format);
    println();
}
void Message::println(uint32_t data, Format format) {
    print(data, format);
    println();
}
void Message::println(int64_t data, Format format) {
    print(data, format);
    println();
}
void Message::println(uint64_t data, Format format) {
    print(data, format);
    println();
}
void Message::println(double data, int digit) {
    print(data, digit);
    println();
}

void Message::printBool(bool ptr) {
    if (ptr) {
        print("true");
    } else {
        print("false");
    }
}

void Message::printlnBool(bool ptr) {
    printBool(ptr);
    println();
}
bool Message::isForAll() const {
    return destinationId == 0;
}


bool Message::isForMe(const size_t& myId) const {
    if (isForAll())
        return true;
    if (destinationId == myId)
        return true;
    return false;
}

}// namespace obd::core::driver
