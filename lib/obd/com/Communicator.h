/**
 * @file Communicator.h
 * @author argawaen
 * @date 03/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "gfx/Color.h"
#include "math/Point.h"
#include "Output.h"
#include <string>
/**
 * @namespace obd
 * @brief Base namespace of the project
 */
/**
 * @brief Namespace gathering Communication
 */
namespace obd::com {

/**
 * @brief Verbosity levels definitions
 */
enum struct Verbosity {
  Error,
  Warning,
  Status,
  Message,
  Extra,
  Debug,
  Trace,
};

/**
 * @brief Class to handle the communication with the rest of the world
 */
class Communicator {
public:
  /**
   * @brief Initialize the communicator
   * @param verbosity The global verbosity level
   * @param withTimeStamp If the time stamps must be printed.
   */
  void begin(const Verbosity &verbosity = Verbosity::Message,
             bool withTimeStamp = false);

  /**
   * @brief Define the verbosity level
   * @param verbosity The new verbosity level
   */
  void setVerbosityLevel(const Verbosity &verbosity = Verbosity::Message) {
    verbosityLevel = verbosity;
  }

  /**
   * @brief Define if timestamps should be printed
   * @param withTimeStamp If the timestamps should be printed.
   */
  void setTimeStamp(bool withTimeStamp = false) { timeStamps = withTimeStamp; }

  /**
   * @brief Start a new line of print
   * @param verbosity The verbosity of the message in the line
   */
  void newline(const Verbosity &verbosity);

  /**
   * @brief Check if the current messge has to be printed
   * @return True if print needed
   */
  [[nodiscard]] bool doPrint() const {
    return currentVerbosity <= verbosityLevel;
  }

  /**
   * @brief Print a string
   * @param s The string to print
   */
  void print(const std::string &s);
  /**
   * @brief Print an uint8_t
   * @param i The uint8_t to print
   * @param format Which format use to print.
   */
  void print(uint8_t i, const Format &format = Format::Auto);
  /**
   * @brief Print an uint16_t
   * @param i The uint16_t to print
   * @param format Which format use to print.
   */
  void print(uint16_t i, const Format &format = Format::Auto);
  /**
   * @brief Print an uint32_t
   * @param i The uint32_t to print
   * @param format Which format use to print.
   */
  void print(uint32_t i, const Format &format = Format::Auto);
  /**
   * @brief Print an uint64_t
   * @param i The uint64_t to print
   * @param format Which format use to print.
   */
  void print(uint64_t i, const Format &format = Format::Auto);
  /**
   * @brief Print an int8_t
   * @param i The int8_t to print
   */
  void print(int8_t i);
  /**
   * @brief Print an int16_t
   * @param i The int16_t to print
   */
  void print(int16_t i);
  /**
   * @brief Print an int32_t
   * @param i The int32_t to print
   */
  void print(int32_t i);
  /**
   * @brief Print an int64_t
   * @param i The int64_t to print
   */
  void print(int64_t i);
  /**
   * @brief Print a float with the desired amount digit
   * @param i The float to print
   * @param digit The amount digits
   */
  void print(float i, uint8_t digit = 2);
  /**
   * @brief Print a double with the desired amount digit
   * @param i The double to print
   * @param digit The amount digits
   */
  void print(double i, uint8_t digit = 2);
  /**
   * @brief Print a Point
   * @param p The Point to print
   */
  void print(const math::Point &p);
  /**
   * @brief Print a Color
   * @param col The Color to print
   */
  void print(const gfx::Color &col);

private:
  /// Actual Verbosity of the message
  Verbosity currentVerbosity = Verbosity::Message;
  /// Actual Verbosity of the communicator
  Verbosity verbosityLevel = Verbosity::Message;
  /// If timestamps should be printed
  bool timeStamps = false;
};

} // namespace cpt::com

/// Real Implementation of the class
extern obd::com::Communicator Port;
