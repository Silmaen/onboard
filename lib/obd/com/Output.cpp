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
#include <sstream>
#endif


namespace obd::com {

#ifdef ARDUINO
int formatToInt(const com::Format& format) {
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        return 10;
    case Format::Hexadecimal:
        return 16;
    case Format::Binary:
        return 2;
    }
}
#endif

void Output::print(const std::string& data) {
    for (const auto& inside : data) {
        write(inside);
    }
}
void Output::print(const char* data) {
#ifdef ARDUINO
    _internalPrint->print(data);
    return;
#endif
    print(std::string(data));
}
void Output::print(int8_t data, com::Format format) {
#ifdef ARDUINO
    _internalPrint->print(data, formatToInt(format));
    return;
#endif
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
        std::cout << data;
        break;
    case Format::Decimal:
        oss << static_cast<int>(data);
        break;
    case Format::Hexadecimal:
        oss << std::setfill('0') << std::setw(2) << std::hex << data;
        break;
    case Format::Binary:
        std::bitset<8> bitset(data);
        oss << bitset;
        break;
    }
    print(oss.str());
}

void Output::print(uint8_t data, com::Format format) {
#ifdef ARDUINO
    _internalPrint->print(data, formatToInt(format));
    return;
#endif
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
        std::cout << data;
        break;
    case Format::Decimal:
        oss << static_cast<int>(data);
        break;
    case Format::Hexadecimal:
        oss << std::setfill('0') << std::setw(2) << std::hex << data;
        break;
    case Format::Binary:
        std::bitset<8> bitset(data);
        oss << bitset;
        break;
    }
    print(oss.str());
}

void Output::print(int16_t data, com::Format format) {
#ifdef ARDUINO
    _internalPrint->print(data, formatToInt(format));
    return;
#endif
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<uint8_t>(data >> 8U), format);
        print(static_cast<uint8_t>(data & 0xff), format);
        break;
    }
}
void Output::print(uint16_t data, com::Format format) {
#ifdef ARDUINO
    _internalPrint->print(data, formatToInt(format));
#endif
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<uint8_t>(data >> 8U), format);
        print(static_cast<uint8_t>(data & 0xff), format);
        break;
    }
}
void Output::print(int32_t data, com::Format format) {
#ifdef ARDUINO
    _internalPrint->print(data, formatToInt(format));
    return;
#endif
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<uint8_t>((data >> 24U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 16U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 8U) & 0xff), format);
        print(static_cast<uint8_t>(data & 0xff), format);
        break;
    }
}
void Output::print(uint32_t data, com::Format format) {
#ifdef ARDUINO
    _internalPrint->print(data, formatToInt(format));
    return;
#endif
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<uint8_t>((data >> 24U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 16U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 8U) & 0xff), format);
        print(static_cast<uint8_t>(data & 0xff), format);
        break;
    }
}
void Output::print(int64_t data, com::Format format) {
#ifdef ARDUINO
    _internalPrint->print(data, formatToInt(format));
    return;
#endif
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<uint8_t>((data >> 56U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 48U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 40U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 32U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 24U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 16U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 8U) & 0xff), format);
        print(static_cast<uint8_t>(data & 0xff), format);
        break;
    }
}
void Output::print(uint64_t data, com::Format format) {
#ifdef ARDUINO
    _internalPrint->print(data, formatToInt(format));
    return;
#endif
    std::stringstream oss;
    switch (format) {
    case Format::Auto:
    case Format::Decimal:
        oss << data;
        print(oss.str());
        break;
    case Format::Hexadecimal:
    case Format::Binary:
        print(static_cast<uint8_t>((data >> 56U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 48U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 40U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 32U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 24U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 16U) & 0xff), format);
        print(static_cast<uint8_t>((data >> 8U) & 0xff), format);
        print(static_cast<uint8_t>(data & 0xff), format);
        break;
    }
}
void Output::print(double data, int digit) {
#ifdef ARDUINO
    _internalPrint->print(data, digit);
    return;
#endif
    std::stringstream oss;
    oss << std::fixed << std::setprecision(digit) << data;
    print(oss.str());
}

void obd::com::Output::println() {
    write('\n');
}

void obd::com::Output::println(const std::string& data) {
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
#ifdef ARDUINO
size_t Output::write(uint8_t data) {
    return _internalPrint->write(data);
}

void Output::print(const Printable& data) {
    _internalPrint->print(data);
}

void Output::print(const __FlashStringHelper* data) {
    _internalPrint->print(data);
}

void Output::print(const String& data) {
    _internalPrint->print(data);
}

void Output::println(const Printable& data) {
    print(data);
    println();
}

void Output::println(const __FlashStringHelper* data) {
    print(data);
    println();
}

void Output::println(const String& data) {
    print(data);
    println();
}
#else
size_t Output::write(uint8_t data) {
    std::cout << data;
    return 1;
}
#endif

}// namespace obd::com
