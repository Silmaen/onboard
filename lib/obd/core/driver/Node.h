/**
 * @file Node.h
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

class Messenger;

/**
 * @brief Node's category
 */
enum struct Category {
    None,
    Hardware,
    Console,
    Communicator,
};

/**
 * @brief Base generic class for driver
 */
class Node: public base::Object {
public:
    Node()            = delete;
    Node(const Node&) = delete;
    Node(Node&&)      = delete;
    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) = delete;
    ~Node()         override = default;
    /// Base Message type
    using Message = obd::core::driver::Message;
    /// Base Message type
    using MessageType = obd::core::driver::Message::MessageType;
    /// Base messenger type
    using Messenger = obd::core::driver::Messenger;
    /// Node's category
    using Category = obd::core::driver::Category;
    /**
     * @brief Default constructor.
     * @param messenger The link to the messenger system
     */
    explicit Node(std::shared_ptr<Messenger> messenger);

    /**
     * @brief Initialize the driver
     */
    void init() override;

    /**
     * @brief Send a message to this driver
     * @param message The message to send
     * @return True mean message caught.
     */
    bool pushMessage(const Message& message);

    /**
     * @brief Get the size of the messages queue
     * @return The size of the queue
     */
    [[nodiscard]] size_t queueSize() const { return messages.size(); }

    /**
     * @brief Return the driver infos
     * @return The driver's infos.
     */
    [[nodiscard]] virtual OString info()const;

    /**
     * @brief Frame Object
     */
    void update() final;

    /**
     * @brief Load the configuration from file
     */
    virtual void loadConfig();

    /**
     * @brief Save the drive configuration in a file
     */
    virtual void saveConfig()const ;

    /**
     * @brief Get the length of message queue
     * @return The length of message queue
     */
    [[nodiscard]] size_t messageSize() const { return messages.size(); }

    /**
     * @brief Try to link the given node
     * @param node The node to link to this one
     * @return True if linked
     */
    virtual bool linkNode(const std::shared_ptr<Node>& node);

    /**
     * @brief Get the node's category
     * @return Node's category
     */
    [[nodiscard]] virtual Category category()const{return Category::None;}


private:
    /// Maximum amount of message treated in one frame
    uint8_t maxFrameMessages = 3;
    /// Pointer to messenger
    std::shared_ptr<Messenger> messenger = nullptr;
    /// List of messages
    std::queue<Message> messages;
protected:
    /**
     * @brief Link to the message list
     * @return the message list
     */
    std::queue<Message>& getMessages(){return messages;}

    /**
     * @brief Link to messenger
     * @return The messenger
     */
    std::shared_ptr<Messenger>& getMessenger(){return messenger;}

    /**
     * @brief Get the name from node Id
     * @param nodeId The nodeId
     * @return The name of node
     */
    OString computeName(const size_t nodeId);

    /**
     * @brief What to do before message treatment
     */
    virtual void preTreatment();

    /**
     * @brief Treat the given message
     * @param message The message to treat
     * @return True if message treated
     */
    virtual bool treatMessage(const Message& message);

    /**
     * @brief What to do after message treatment
     */
    virtual void postTreatment();

    /**
     * @brief Send a message to this driver
     * @param message The Command message to send
     * @return True mean command caught.
     */
    virtual bool pushCommand(const Message& message);

    /**
     * @brief Get the Id of the Shell
     * @return The shell Id
     */
    static size_t getConsoleId();

    /**
     * @brief Send a message to other
     * @param message The message to send
     */
    void broadcastMessage(const Message& message) const;

    /**
     * @brief Send a message to other
     * @param destination The destination id
     * @param msg The message to send
     * @param type The message's type
     */
    void broadcastMessage(const size_t& destination,const OString& msg, Message::MessageType type=Message::MessageType::Message) const;

    /**
     * @brief Push a console message
     * @param msg The message to push
     * @param type The message's type
     */
    void console(const OString& msg, Message::MessageType type=Message::MessageType::Message) const;
};

}// namespace obd::core::driver
