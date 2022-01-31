/**
 * @file Messenger.h
 * @author argawaen
 * @date 14/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Message.h"
#include "core/base/Object.h"
#include <memory>
#include <queue>

namespace obd::core::driver {

class Manager;

/**
 * @brief Structure carrying stats
 */
struct Statistics {
    uint64_t receivedMessages = 0;///< Amount of received message
    uint64_t sentMessages     = 0;///< Ammount of dropped message
    uint64_t droppedMessage   = 0;///< Amount of dropped message
};

/**
 * @brief Class to handle the message trafic
 */
class Messenger : public base::Object {
public:
    /**
     * @brief Default constructor.
     * @param manager Link to the node manager
     */
    explicit Messenger(std::shared_ptr<Manager> manager = nullptr);

    /**
     * @brief Add message to sending list
     * @param message
     */
    void pushMessage(const Message& message);

    void init() override;

    /**
     * @brief Update the treatment
     */
    void update() override;

    bool check() override;

    /**
     * @brief Get the actual queue size
     * @return The queue size
     */
    [[nodiscard]] size_t size() const { return messages.size(); }

    /**
     * @brief Get the actual trafic statistics
     * @return The stats
     */
    [[nodiscard]] const Statistics& stats() const { return statistics; }

    /**
     * @brief Get the name from node Id
     * @param nodeId The nodeId
     * @return The name of node
     */
    OString computeName(const size_t nodeId);

    /**
     * @brief Compute the ID base on the name
     * @param name The node's name
     * @return The node's ID, return 0 if node not found
     */
    size_t computeId(const OString& name);

    /**
     * @brief
     * @return
     */
    std::vector<OString> getDriverList()const;
private:
    /// Maximum amount of message treated in one frame
    uint8_t maxFrameMessages = 10;
    /// Pointer to the Manager
    std::shared_ptr<Manager> manager;
    /// Messages list
    std::queue<Message> messages;
    /// The stats
    Statistics statistics;

    /**
     * @brief Send the message to the designated target(s)
     * @param message The message
     * @return True if message effectively sent
     */
    bool sendMessage(const Message& message);
};

}// namespace obd::core::driver
