/**
 * @file Message.h
 * @author argawaen
 * @date 14/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "native/OString.h"
#include <vector>
/**
 * @brief Namespace for hardware management
 */
namespace obd::core::driver {


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
 * @brief Definition of a message for a driver
 */
class Message {
public:
    /// Data type
    using DataType = OString;
    /// List of data
    using MultipleData = std::vector<DataType>;
    /// Format of integers
    using Format = obd::core::driver::Format;

    /// message's types
    enum struct MessageType {
        Command,///< Some action to do
        Reply,  ///< Reply to a command
        Message,///< A console message
        Warning,///< A warning to treat, log and send to consoles
        Error,  ///< An error to treat, log, and send to consoles
        Input,  ///< Input message from exterior
    };


    /**
     * @brief Constructor with command
     * @param src The source's name of the message
     * @param dest The destination's name of the message
     * @param type The message's type
     */
    Message(const size_t& src, const size_t& dest, const MessageType& type = MessageType::Message) :
        messageType{type}, sourceId{src},destinationId{dest} {}

    /**
     * @brief Constructor with command
     * @param src The source's id of the message
     * @param dest The destination's id of the message
     * @param cmd The command
     * @param type The message's type
     */
    Message(const size_t& src, const size_t& dest, const DataType& cmd, const MessageType& type = MessageType::Message) :
        messageType{type}, sourceId{src}, destinationId{dest}, message{cmd} {}

    /**
     * @brief Definition of the message
     * @param cmd The new message
     */
    void setMessage(const DataType& cmd) {
        message = cmd;
    }

    /**
     * @brief Get the command
     * @return The command
     */
    [[nodiscard]] const DataType& getMessage() const {
        return message;
    }

    /**
     * @brief Definition of the message
     * @param type The new message's type
     */
    void setType(const MessageType& type) {
        messageType = type;
    }

    /**
     * @brief Get the command
     * @return The command
     */
    [[nodiscard]] const MessageType& getType() const {
        return messageType;
    }
    /**
     * @brief Get the base of the command (first word of full command)
     * @return The base command
     */
    [[nodiscard]] DataType getBaseCommand() const;

    /**
     * @brief Get the param of the command (full command without first word)
     * @return The command's param
     */
    [[nodiscard]] DataType getParamStr() const;

    /**
     * @brief Does the command have parameters
     * @return True if parameters
     */
    [[nodiscard]] bool hasParams() const;

    /**
     * @brief Get the list of parameters
     * @return The list of parameters
     */
    [[nodiscard]] MultipleData getParams() const;

    /**
     * @brief Get the command
     * @return The command
     */
    [[nodiscard]] const size_t& getSource() const {
        return sourceId;
    }

    /**
     * @brief Get the command
     * @return The command
     */
    [[nodiscard]] const size_t& getDestination() const {
        return destinationId;
    }

    /**
     * @brief Clear the message
     */
    void clear() {
        message.clear();
    }

    /**
     * @brief Check emptiness
     * @return True if empty
     */
    [[nodiscard]] bool empty() const {
        return message.empty();
    }

    /**
     * @brief Check if message
     * @return True if message
     */
    [[nodiscard]] bool isMessage()const {
        return messageType == MessageType::Error || messageType == MessageType::Warning || messageType == MessageType::Message;
    }

    /**
     * @brief Check if broadcast message
     * @return Tre if broadcast message
     */
    [[nodiscard]] bool isForAll()const;

    /**
     * @brief Check if the message meets destination
     * @param myId The destination Id
     * @return True if good this destination
     */
    [[nodiscard]] bool isForMe(const size_t& myId)const;
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
    void print(int8_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(uint8_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(int16_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(uint16_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(int32_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(uint32_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(int64_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer
     * @param data The integer to print
     * @param format The int format
     */
    void print(uint64_t data, Format format = Format::Auto);
    /**
     * @brief Print an float
     * @param data The float to print
     * @param digit The amount of digit to print
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
    void println(int8_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(uint8_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(int16_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(uint16_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(int32_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(uint32_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(int64_t data, Format format = Format::Auto);
    /**
     * @brief Print an integer with a end of line
     * @param data The integer to print
     * @param format The int format
     */
    void println(uint64_t data, Format format = Format::Auto);
    /**
     * @brief Print an float
     * @param data The float to print
     * @param digit The amount of digit to print
     */
    void println(double data, int digit = 2);

    /**
     * @brief Print bool value as [true, false]
     * @param ptr The bool value
     */
    void printBool(bool ptr);

    /**
     * @brief Print bool value as [true, false] with a newline
     * @param ptr The bool value
     */
    void printlnBool(bool ptr);

private:
    /// The message's type
    MessageType messageType = MessageType::Message;
    /// The hash code of the source driver
    size_t sourceId;
    /// The hash code of the destination driver
    size_t destinationId;
    /// The content of the message
    DataType message;
};

}// namespace obd::core::driver
