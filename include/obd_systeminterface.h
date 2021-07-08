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
 * @brief base class for interfacing with the system
 */
class SystemInterface {
public:
    /**
     * @brief constructor with parent
     * @param p the parent system
     */
    explicit SystemInterface(System* p = nullptr) :
    parent{p} {}

    /**
     * @brief get the parent system
     * @return the parent
     */
    [[nodiscard]] System* getParent() const {
        return parent;
    }

    /**
     * @brief get the outputs from the parent system
     * @return the output
     */
    [[nodiscard]] Print* getParentPrint() const;

    /**
     * @brief try to print in MultiStream
     * @param in input to display
     * @return number of byte writen
     */
    void print(const __FlashStringHelper* in) const;

    /**
     * @brief try to print in MultiStream
     * @param in input to display
     * @return number of byte writen
     */
    void print(const String& in) const;

    /**
     * @brief try to print in MultiStream
     * @param in input to display
     * @return number of byte writen
     */
    void print(const char* in) const;

    /**
     * @brief try to print in MultiStream
     * @param in input to display
     * @return number of byte writen
     */
    void print(char in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void print(unsigned char in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void print(int16_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void print(uint16_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void print(int32_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void print(uint32_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void print(int64_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void print(uint64_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void print(double in, int format = 2) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    void print(const Printable& in) const;


    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    void println(const __FlashStringHelper* in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    void println(const String& in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    void println(const char* in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    void println(char in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void println(unsigned char in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void println(int16_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void println(uint16_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void println(int32_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void println(uint32_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void println(int64_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void println(uint64_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    void println(double in, int format = 2) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    void println(const Printable& in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @return number of byte writen
     */
    void println() const;

    /**
     * @brief try to add a command int eh parent
     * @param cmd command to add
     */
    void pushCommand(const command& cmd) const;

    /**
     * @brief Print a bool value as [true, false]
     * @param ptr the bool value
     */
    void printBool(bool ptr) const;

    /**
     * @brief Print a bool value as [true, false] with a newline
     * @param ptr the bool value
     */
    void printlnBool(bool ptr) const;
private:
    /// pointer to the parent system
    System* parent = nullptr;
};

}// namespace obd::core

