/**
 * @file SystemInterface.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <memory>
#include "com/Output.h"
#include "native/fakeArduino.h"
#include "Command.h"

namespace obd::core {

class System;

/**
 * @brief Base class for interfacing with the system
 */
class SystemInterface {
public:
    /**
     * @brief Constructor with parent
     * @param parentSystem The parent system
     */
    explicit SystemInterface(System* parentSystem = nullptr) :
        parent{parentSystem} {}

    /**
     * @brief Get the parent system
     * @return The parent
     */
    [[nodiscard]] System* getParent() const {
        return parent;
    }

    /**
     * @brief Get the outputs from the parent system
     * @return The output
     */
    [[nodiscard]] com::Output* getParentOutput() const;

#ifdef ARDUINO
    /**
     * @brief Try to print in MultiStream
     * @param in Input to display
     */
    void print(const __FlashStringHelper* in) const;

    /**
     * @brief Try to print in MultiStream
     * @param in Input to display
     */
    void print(const String& in) const;
#endif

    /**
     * @brief Try to print in MultiStream
     * @param in Input to display
     */
    void print(const std::string& in) const;

    /**
     * @brief Try to print in MultiStream
     * @param in Input to display
     */
    void print(const char* in) const;

    /**
     * @brief Try to print in MultiStream
     * @param in Input to display
     */
    void print(char in) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(unsigned char in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(int16_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(uint16_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(int32_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(uint32_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(int64_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(uint64_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(double in, int format = 2) const;

#ifdef ARDUINO
    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     */
    void print(const Printable& in) const;


    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     */
    void println(const __FlashStringHelper* in) const;


    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     */
    void println(const String& in) const;
#endif

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     */
    void println(const std::string& in) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     */
    void println(const char* in) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     */
    void println(char in) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(unsigned char in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(int16_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(uint16_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(int32_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(uint32_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(int64_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(uint64_t in, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(double in, int format = 2) const;

#ifdef ARDUINO
    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     */
    void println(const Printable& in) const;
#endif

    /**
     * @brief Try to print in MultiStream with a newline
     */
    void println() const;

    /**
     * @brief Try to add a command in the parent
     * @param cmd Command to add
     */
    void pushCommand(const Command& cmd) const;

    /**
     * @brief Print bool value as [true, false]
     * @param ptr The bool value
     */
    void printBool(bool ptr) const;

    /**
     * @brief Print bool value as [true, false] with a newline
     * @param ptr The bool value
     */
    void printlnBool(bool ptr) const;

private:
    /// Pointer to the parent system
    System* parent = nullptr;
};

}// namespace obd::core
