/**
 * @file Communicator.cpp
 * @author argawaen
 * @date 03/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Communicator.h"
#include "native/fakeArduino.h"
#include <bitset>
#include <iomanip>
#include <iostream>

obd::com::Communicator Port;

namespace obd::com {

/// Default baud rate for the communications
constexpr uint32_t defaultBaudRate = 115200;

void Communicator::begin(const Verbosity &verbosity, bool withTimeStamp) {
#ifdef Arduino
  Serial.begin(defaultBaudRate);
  Serial.println();
#else
  std::cout << std::endl;
#endif
  setVerbosityLevel(verbosity);
  setTimeStamp(withTimeStamp);
}

void Communicator::newline(const Verbosity &verbosity) {
  currentVerbosity = verbosity;
  if (currentVerbosity > verbosityLevel)
    return;
#ifdef Arduino
  Serial.println();
#else
  std::cout << std::endl;
#endif
  if (timeStamps) {
    uint64_t mil = millis();
    uint32_t sec = mil / 1000;
#ifdef Arduino
    Serial.print("[");
    if (sec > 0) {
      mil -= 1000 * sec;
      Serial.print(sec);
      Serial.print(".");
    }
    Serial.print(mil);
    Serial.print("] ");
#else
    std::cout << "[";
    if (sec > 0) {
      mil -= 1000 * sec;
      std::cout << sec << ".";
    }
    std::cout << mil << "] ";
#endif
  }
  std::string keyword;
  switch (currentVerbosity) {
  case Verbosity::Error:
    keyword = "ERROR ";
    break;
  case Verbosity::Warning:
    keyword = "WARNING ";
    break;
  case Verbosity::Status:
    keyword = "STATUS ";
    break;
  case Verbosity::Message:
    keyword = "MESSAGE ";
    break;
  case Verbosity::Extra:
    keyword = "EXTRA ";
    break;
  case Verbosity::Debug:
    keyword = "DEBUG ";
    break;
  case Verbosity::Trace:
    keyword = "TRACE ";
    break;
  }
#ifdef ARDUINO
  Serial.print(keyword.c_str());
#else
  std::cout << keyword;
#endif
}

void Communicator::print(const std::string &s) {
  if (currentVerbosity > verbosityLevel)
    return;
#ifdef ARDUINO
  Serial.print(s.c_str());
#else
  std::cout << s;
#endif
}
void Communicator::print(uint8_t i, const Format &format) {
  if (currentVerbosity > verbosityLevel)
    return;
  switch (format) {
  case Format::Auto:
#ifdef ARDUINO
    Serial.print(i);
#else
    std::cout << i;
#endif
    break;
  case Format::Decimal:
#ifdef ARDUINO
    Serial.print(i, DEC);
#else
    std::cout << (int)i;
#endif
    break;
  case Format::Hexadecimal:
#ifdef ARDUINO
    if (i < 0x10)
      Serial.print("0");
    Serial.print(i, HEX);
#else
    std::cout << std::setfill('0') << std::setw(2) << std::hex << i;
#endif
    break;
  case Format::Binary:
#ifdef ARDUINO
    if (i < 0b1000000)
      Serial.print("0");
    if (i < 0b100000)
      Serial.print("0");
    if (i < 0b10000)
      Serial.print("0");
    if (i < 0b1000)
      Serial.print("0");
    if (i < 0b100)
      Serial.print("0");
    if (i < 0b10)
      Serial.print("0");
    Serial.print(i, BIN);
#else
    std::bitset<8> x(i);
    std::cout << x;
#endif
    break;
  }
}
void Communicator::print(uint16_t i, const Format &format) {
  if (currentVerbosity > verbosityLevel)
    return;
  switch (format) {
#ifdef ARDUINO
  case Format::Auto:
    Serial.print(i);
    break;
  case Format::Decimal:
    Serial.print(i, DEC);
#else
  case Format::Auto:
  case Format::Decimal:
    std::cout << i;
#endif
    break;
  case Format::Hexadecimal:
  case Format::Binary:
    print((uint8_t)(i >> 8U), format);
    print((uint8_t)(i & 0xff), format);
    break;
  }
}
void Communicator::print(uint32_t i, const Format &format) {
  if (currentVerbosity > verbosityLevel)
    return;
  switch (format) {
#ifdef ARDUINO
  case Format::Auto:
    Serial.print(i);
    break;
  case Format::Decimal:
    Serial.print(i, DEC);
#else
  case Format::Auto:
  case Format::Decimal:
    std::cout << i;
#endif
    break;
  case Format::Hexadecimal:
  case Format::Binary:
    print((uint8_t)((i >> 24U) & 0xff), format);
    print((uint8_t)((i >> 16U) & 0xff), format);
    print((uint8_t)((i >> 8U) & 0xff), format);
    print((uint8_t)(i & 0xff), format);
    break;
  }
}
void Communicator::print(uint64_t i, const Format &format) {
  if (currentVerbosity > verbosityLevel)
    return;
  switch (format) {
#ifdef ARDUINO
  case Format::Auto:
    Serial.print(i);
    break;
  case Format::Decimal:
    Serial.print(i, DEC);
#else
  case Format::Auto:
  case Format::Decimal:
    std::cout << i;
#endif
    break;
  case Format::Hexadecimal:
  case Format::Binary:
    print((uint8_t)((i >> 56U) & 0xff), format);
    print((uint8_t)((i >> 48U) & 0xff), format);
    print((uint8_t)((i >> 40U) & 0xff), format);
    print((uint8_t)((i >> 32U) & 0xff), format);
    print((uint8_t)((i >> 24U) & 0xff), format);
    print((uint8_t)((i >> 16U) & 0xff), format);
    print((uint8_t)((i >> 8U) & 0xff), format);
    print((uint8_t)(i & 0xff), format);
    break;
  }
}
void Communicator::print(int8_t i) {
  if (currentVerbosity > verbosityLevel)
    return;
#ifdef ARDUINO
  Serial.print(i);
#else
  std::cout << i;
#endif
}
void Communicator::print(int16_t i) {
  if (currentVerbosity > verbosityLevel)
    return;
#ifdef ARDUINO
  Serial.print(i);
#else
  std::cout << i;
#endif
}
void Communicator::print(int32_t i) {
  if (currentVerbosity > verbosityLevel)
    return;
#ifdef ARDUINO
  Serial.print(i);
#else
  std::cout << i;
#endif
}
void Communicator::print(int64_t i) {
  if (currentVerbosity > verbosityLevel)
    return;
#ifdef ARDUINO
  Serial.print(i);
#else
  std::cout << i;
#endif
}
void Communicator::print(float i, uint8_t digit) {
  if (currentVerbosity > verbosityLevel)
    return;
#ifdef ARDUINO
  Serial.print(i, digit);
#else
  std::cout << std::fixed << std::setprecision(digit) << i;
#endif
}
void Communicator::print(double i, uint8_t digit) {
  if (currentVerbosity > verbosityLevel)
    return;
#ifdef ARDUINO
  Serial.print(i, digit);
#else
  std::cout << std::fixed << std::setprecision(digit) << i;
#endif
}

void Communicator::print(const math::Point &p) {
  if (currentVerbosity > verbosityLevel)
    return;
#ifdef ARDUINO
  Serial.print("(");
  Serial.print(p.x);
  Serial.print(",");
  Serial.print(p.y);
  Serial.print(")");
#else
  std::cout << "(" << p.x << "," << p.y << ")";
#endif
}
void Communicator::print(const gfx::Color &col) {
  if (currentVerbosity > verbosityLevel)
    return;
#ifdef ARDUINO
  Serial.print("#");
#else
  std::cout << "#";
#endif
  print(col.red, Format::Hexadecimal);
  print(col.green, Format::Hexadecimal);
  print(col.blue, Format::Hexadecimal);
}
} // namespace cpt::com
