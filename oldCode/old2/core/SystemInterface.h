/**
 * @file SystemInterface.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Command.h"
#include "com/Output.h"
#include "native/fakeArduino.h"
#include "com/ConsoleOutput.h"
#include <memory>
#include <utility>

namespace obd::core {

class System_old;

/**
 * @brief Base class for interfacing with the system
 */
class SystemInterface {
public:
    /**
     * @brief Constructor with parent
     * @param parentSystem The parent system
     */
    explicit SystemInterface(System_old* parentSystem) :
        parent{parentSystem} {}

    /**
     * @brief Try to print in MultiStream
     * @param data Input to display
     */
    void print(const OString& data) const;

    /**
     * @brief Try to print in MultiStream
     * @param data Input to display
     */
    void print(const char* data) const;

    /**
     * @brief Try to print in MultiStream
     * @param data Input to display
     */
    void print(char data) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void print(unsigned char data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void print(int16_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void print(uint16_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void print(int32_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void print(uint32_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void print(int64_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void print(uint64_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param digits Format of the display
     */
    void print(double data, int digits = 2) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     */
    void println(const OString& data) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     */
    void println(const char* data) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     */
    void println(char data) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void println(unsigned char data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void println(int16_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void println(uint16_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void println(int32_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void println(uint32_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void println(int64_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param format Format of the display
     */
    void println(uint64_t data, com::Format format = com::Format::Auto) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param data Input to display
     * @param digit Format of the display
     */
    void println(double data, int digit = 2) const;

    /**
     * @brief Try to print in MultiStream with a newline
     */
    void println() const;

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

    /**
     * @brief Try to add a command in the parent
     * @param cmd Command to add
     */
    void pushCommand(const Command& cmd) const;

    /**
     * @brief Check the parent link
     * @return True if linked to parent
     */
    [[nodiscard]] bool isLinked()const{return parent!=nullptr;}

    /**
     * @brief Add an output to the system
     * @param print the output to add
     * @return True if the output added
     */
    bool addOutput(const std::shared_ptr<com::Output>& print);

    /**
     * @brief Get the system timestamp
     * @return The system timestamp
     */
    [[nodiscard]] uint64_t getTimestamp()const;

    /**
     * @brief Get the parent system
     * @return The parent
     */
    [[nodiscard]] System_old* getParent() const {
        return parent;
    }
private:


    /**
     * @brief Get the outputs from the parent system
     * @return The output
     */
    [[nodiscard]] com::Output& getParentOutput() const;

    /// Pointer to the parent system
    System_old* parent;
};

}// namespace obd::core
