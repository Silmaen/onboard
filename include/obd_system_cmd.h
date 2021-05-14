//
// Created by damien.lachouette on 11/05/2021.
//

#pragma once
#include <Arduino.h>
#include <config.h>

namespace obd {
namespace system {

/**
 * @brief type of the sources
 */
enum class source {
    NONE,  ///< no source
    USB,   ///< USB source
    UART_0,///< Serial source
    TELNET ///< Telnet source
};


/**
 * @brief class to handle command message
 */
class command {
public:
    command() = default;
    ~command() = default;
    command(const command &) = default;
    command(command &&) = default;
    command &operator=(const command &) = default;
    command &operator=(command &&) = default;
    /**
     * @brief constructor with source
     * @param src the source of the message
     */
    explicit command(const source &src):from{src} {}

    /**
     * @brief define the source of the command
     * @param src the new source of the command
     */
    void setSource(const source &src) { from = src; }

    /**
     * @brief reset the command line
     */
    void reset() {
        curr_ptr = 0;
        cmdline[curr_ptr] = '\0';
    }

    /**
     * @brief get the size of the command
     * @return the size of the message
     */
    uint8_t size() const { return curr_ptr; }

    /**
     * @brief add a char to the command
     * @param c the char to add
     */
    bool putChar(char c) {
        cmdline[curr_ptr++] = c;
        cmdline[curr_ptr] = '\0';
        return curr_ptr < commandBufferLength;
    }

    /**
     * @brief get the c-string
     * @return the command string
     */
    const char *c_str() const { return cmdline; }

    /**
     * @brief determine if this command match the given one
     * @param cmp the command to compare
     * @return true if commands matches
     */
    bool isCmd(const char *cmp);

    /**
     * @brief print in the given output
     * @param st the output
     */
    void printCmd(Print &st);

    /**
     * @brief get the parameter string
     * @return the parameter string may be null string
     */
    const char* getParams()const;
private:
    source from = source::NONE;            ///< the source of the command
    char cmdline[commandBufferLength];     ///< le string of the command line
    uint8_t curr_ptr = 0;                  ///< current pointer
};

}// namespace system
}// namespace obd
