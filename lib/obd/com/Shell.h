/**
 * @file Shell.h
 * @author argawaen
 * @date 19/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <unordered_set>
#include "core/driver/Node.h"

namespace obd::com {

/**
 * @brief The system's shell
 */
class Shell: public core::driver::Node {
public:
    /**
     * @brief Default constructor.
     * @param parent The link to the messenger system
     */
    explicit Shell(std::shared_ptr<Messenger> parent):Node{std::move(parent)}{}

    /**
     * @brief Get the node's category
     * @return Node's category
     */
    [[nodiscard]] Category category() const final { return Category::Console; }

    /**
     * @brief Add an output to the list
     * @param hcd The output's hashcode
     */
    void addOutput(size_t hcd);

    /**
     * @brief Remove an output of the list
     * @param hcd The output's hashcode
     */
    void removeOutput(size_t hcd);

private:
    /// List of Output com nodes ids.
    std::unordered_set<size_t> outputs;

    void outputMessage(const Message& message);

    void outputMessage(const OString& message, const MessageType& type);

    bool treatMessage(const Message& message)override;

    bool shellCommand(const Message& message);

    void dmesg();

    void lsdrv();
};
}// namespace obd::com
