/**
 * @file Output.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "native/fakeArduino.h"
#include <string>

/**
 * @brief Function for communication
 */
namespace obd::com {

/**
 * @brief Output format for integers
 */
enum struct Format {
    Auto,
    Decimal,
    Hexadecimal,
    Binary,
};

/**
 * @brief Class handling Output to console
 */
class Output {
public:
    Output(const Output&) = delete;
    Output(Output&&)      = delete;
    Output& operator=(const Output&) = delete;
    Output& operator=(Output&&) = delete;

    /**
     * @brief Default Constructor
     */
    Output() = default;

    /// Destructor
    virtual ~Output() = default;
    /**
     * @brief Print a string
     * @param data The String to print
     */
    void print(const OString& data);
    /**
     * @brief Print a string
     * @param data The String to print
     */
    void print(const char* data);
    /**
     * @brief Print a string
     * @param data The String to print
     * @param format The int format
     */
    void print(int8_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(uint8_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(int16_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(uint16_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(int32_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(uint32_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(int64_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(uint64_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an float
     * @param data The float to print
     * @param digit The number of digit to print
     */
    void print(double data, int digit = 2);

    /**
     * @brief Print a new line char
     */
    void println();

    /**
     * @brief Print a string with end line
     * @param data The String to print
     */
    void println(const OString& data);
    /**
     * @brief Print a string with end line
     * @param data The String to print
     */
    void println(const char* data);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(int8_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(uint8_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(int16_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(uint16_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(int32_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(uint32_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(int64_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(uint64_t data, com::Format format = com::Format::Auto);
    /**
     * @brief Print an float
     * @param data The float to print
     * @param digit The number of digit to print
     */
    void println(double data, int digit = 2);

private:
    /**
     * @brief Print a string
     * @param data The String to print
     */
    virtual void writeBytes(const OString& data);
};


}// namespace obd::com
