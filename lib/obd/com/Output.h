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
    /**
     * @brief Default Constructor
     */
    Output() =default;
    /**
     * @brief no copy of this class
     */
    Output(const Output&) = delete;
    /**
     * @brief no move of this class
     */
    Output(Output&&) = delete;
#ifdef ARDUINO
    /**
     * @brief Build an output based of a Print
     * @param print The base print
     */
    explicit Output(Print* print):_internalPrint{print}{}
#endif
    /// Destructor
    virtual ~Output() = default;
    /**
     * @brief Print a string
     * @param data The String to print
     */
    void print(const std::string& data);
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
#ifdef ARDUINO
    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     */
    void print(const Printable& data) ;
    /**
     * @brief Try to print in MultiStream
     * @param data Input to display
     */
    void print(const __FlashStringHelper* data) ;

    /**
     * @brief Try to print in MultiStream
     * @param data Input to display
     */
    void print(const String& data) ;
#endif
    /**
     * @brief Print a new line char
     */
    void println();

    /**
     * @brief Print a string with end line
     * @param data The String to print
     */
    void println(const std::string& data);
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
#ifdef ARDUINO
    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     */
    void println(const Printable& data);
    /**
     * @brief Try to print in MultiStream
     * @param data Input to display
     */
    void println(const __FlashStringHelper* data);

    /**
     * @brief Try to print in MultiStream
     * @param data Input to display
     */
    void println(const String& data);
#endif


    /**
     * @brief Pure virtual method
     * @param data The Byte to write
     * @return The amount of data written
     */
#ifdef ARDUINO
    size_t write(uint8_t data);
#else
    virtual size_t write(uint8_t data);
#endif

private:
#ifdef ARDUINO
    /// internal Arduino Print
    Print* _internalPrint;
#endif
};


}// namespace obd::com
