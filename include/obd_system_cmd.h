/**
 * @author Silmaen
 * @date 11/05/2021
 */
#pragma once

#include "classfwd.h"
#include "obd_inputsource.h"
#include <Print.h>
#include <string>

namespace obd::core {

/**
 * @brief class to handle command message
 */
class command {
public:
    /**
     * @brief constructor with source
     * @param src the source of the message
     */
    explicit command(const network::source& src) :
        from{src} { clear(); }

    /**
     * @brief define the source of the command
     * @param src the new source of the command
     */
    [[maybe_unused]] void setSource(const network::source& src) { from = src; }

    /**
     * @brief reset the command line
     */
    void clear() { cmdline.clear(); }

    /**
     * @brief return if the command is empty
     * @return true if empty
     */
    [[nodiscard]] bool empty() const { return cmdline.isEmpty(); }

    /**
     * @brief Add a char to the command
     * @param c The char to add
     * @return True if command succeed.
     */
    bool putChar(char c) {
        cmdline += c;
        return cmdline.length() < config::commandBufferLength;
    }

    /**
     * @brief determine if this command match the given one
     * @param cmp the command to compare
     * @return true if commands matches
     */
    [[nodiscard]] bool isCmd(const String& cmp) const;

    /**
     * @brief print in the given output
     * @param st the output
     */
    void printCmd(Print& st) const;

    /**
     * @brief get the parameter string
     * @return the parameter string may be null string
     */
    [[nodiscard]] String getParams() const;

private:
    /// the source of the command
    network::source from = network::source::NONE;

    /// the string of the command line
    String cmdline;
};

}// namespace obd::core
