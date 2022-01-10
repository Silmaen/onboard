/**
 * @file Command.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "com/Output.h"
#include "com/Source.h"
#include "config.h"

namespace obd::core {

/**
 * @brief Class handling command message
 */
class Command {
public:
    /**
     * @brief Constructor with source
     * @param src The source of the message
     */
    Command(const com::Source& src=com::Source::NONE) :
        from{src} { clear(); }

    /**
     * @brief Define the source of the command
     * @param src The new source of the command
     */
    [[maybe_unused]] void setSource(const com::Source& src) { from = src; }

    /**
     * @brief Reset the command line
     */
    void clear() { cmdline.clear(); }

    /**
     * @brief Return if the command emptiness
     * @return True if empty
     */
    [[nodiscard]] bool empty() const { return cmdline.empty(); }

    /**
     * @brief Add a char to the command
     * @param character The char to add
     * @return True if command succeed.
     */
    bool putChar(char character) {
        cmdline += character;
        return cmdline.length() < config::commandBufferLength;
    }

    /**
     * @brief Determine if this command match the given one
     * @param cmp The command to compare
     * @return True if commands matches
     */
    [[nodiscard]] bool isCmd(const std::string& cmp) const;

    /**
     * @brief Print in the given output
     * @param output The output
     */
    void printCmd(com::Output& output) const;

    /**
     * @brief Get the parameter string
     * @return The parameter string may be null string
     */
    [[nodiscard]] std::string getParams() const;

private:
    /// The source of the command
    com::Source from = com::Source::NONE;

    /// The string of the command line
    std::string cmdline;
};

}// namespace obd::core
