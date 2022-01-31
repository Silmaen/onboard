/**
 * @file Output.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "Output.h"
#ifndef ARDUINO
#include <bitset>
#include <iomanip>
#include <iostream>
#endif


namespace obd::com {

void Output::print([[maybe_unused]]const OString& data) {
    writeBytes(data);
}

void Output::print(const char* data) {
    print(OString(data));
}

void Output::print(int8_t data, com::Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
        std::cout << data;
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
    print(oss.str());
}

void Output::print(uint8_t data, com::Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
        std::cout << data;
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
    print(oss.str());
}

void Output::print(int16_t data, com::Format format) {
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
    print(oss.str());
}
void Output::print(uint16_t data, com::Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str());
        break;
    case Format::Hexadecimal:
        oss << std::setfill('0') << std::setw(4) << std::hex << data;
        break;
    case Format::Binary:
        std::bitset<16> bitset(data);
        oss << bitset;
        break;
    }
    print(oss.str());
}
void Output::print(int32_t data, com::Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<int16_t>((data >> 16U) & 0xffff), format);
        print(static_cast<int16_t>(data & 0xffff), format);
        break;
    }
}
void Output::print(uint32_t data, com::Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<uint16_t>((data >> 16U) & 0xffff), format);
        print(static_cast<uint16_t>(data & 0xffff), format);
        break;
    }
}
void Output::print(int64_t data, com::Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str());
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
void Output::print(uint64_t data, com::Format format) {
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str());
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
void Output::print(double data, int digit) {
    std::stringstream oss;
    oss << std::fixed << std::setprecision(digit) << data;
    print(oss.str());
}

void obd::com::Output::println() {
    print("\n");
}

void obd::com::Output::println(const OString& data) {
    print(data + "\n");
}

void Output::println(const char* data) {
    print(data);
    println();
}
void Output::println(int8_t data, com::Format format) {
    print(data, format);
    println();
}
void Output::println(uint8_t data, com::Format format) {
    print(data, format);
    println();
}
void Output::println(int16_t data, com::Format format) {
    print(data, format);
    println();
}
void Output::println(uint16_t data, com::Format format) {
    print(data, format);
    println();
}
void Output::println(int32_t data, com::Format format) {
    print(data, format);
    println();
}
void Output::println(uint32_t data, com::Format format) {
    print(data, format);
    println();
}
void Output::println(int64_t data, com::Format format) {
    print(data, format);
    println();
}
void Output::println(uint64_t data, com::Format format) {
    print(data, format);
    println();
}
void Output::println(double data, int digit) {
    print(data, digit);
    println();
}

void Output::writeBytes([[maybe_unused]]const OString& data) {
    // do nothing, this is a dummy output
}

}// namespace obd::com
