/**
 * \author Silmaen
 * \date 06/07/2021
 */

#pragma once
#include "classfwd.h"
#include "obd_system_cmd.h"
#include "obd_drivertypes.h"
#include <Print.h>
#include <memory>

namespace obd::core {

/**
 * @brief Base class for interfacing with the system
 */
class SystemInterface {
public:
    /**
     * @brief Constructor with parent
     * @param p The parent system
     */
    explicit SystemInterface(System* p = nullptr) :
    parent{p} {}

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
    [[nodiscard]] Print* getParentPrint() const;

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
    void print(unsigned char in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(int16_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(uint16_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(int32_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(uint32_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(int64_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(uint64_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void print(double in, int format = 2) const;

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
    void println(unsigned char in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(int16_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(uint16_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(int32_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(uint32_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(int64_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(uint64_t in, int format = DEC) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     * @param format Format of the display
     */
    void println(double in, int format = 2) const;

    /**
     * @brief Try to print in MultiStream with a newline
     * @param in Input to display
     */
    void println(const Printable& in) const;

    /**
     * @brief Try to print in MultiStream with a newline
     */
    void println() const;

    /**
     * @brief Try to add a command int eh parent
     * @param cmd Command to add
     */
    void pushCommand(const command& cmd) const;

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

