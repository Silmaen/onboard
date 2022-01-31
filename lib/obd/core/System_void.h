/**
 * @file System.h
 * @author argawaen
 * @date 14/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "driver/Manager.h"
#include "driver/Messenger.h"

namespace obd::core {

/**
 * @brief Base system
 */
class System_void : public base::Object {
public:
    System_void(const System_void&) = delete;
    System_void(System_void&&)      = delete;
    System_void& operator=(const System_void&) = delete;
    System_void& operator=(System_void&&) = delete;
    /**
     * @brief Default constructor.
     */
    System_void() {}
    /**
     * @brief Destructor.
     */
    ~System_void() override {}

    /**
     * @brief Initialisation
     */
    void init() override {
        Object::init();
        manager= std::make_shared<driver::Manager>();
        messenger = std::make_shared<driver::Messenger>(manager);
        messenger->init();
        manager->init();
    }

    /**
     * @brief Actualization frame
     */
    void update() override {
        if (!initialized()) {
            return;
        }
        messenger->update();
        manager->update();
    }

    /**
     * @brief Do a full system check
     * @return True if everything ok
     */
    [[nodiscard]] bool check() override {
        return initialized();
    }

    /**
     * @brief Add a node
     * @tparam T The node's type to add
     * @return True if the node truly added
     */
    template<class T>
    bool addNode() {
        if (!std::is_base_of<driver::Node, T>::value)// only class derived from Node is allowed
            return false;
        auto temp = std::make_shared<T>(messenger);
        return manager->addNode(temp);
    }

    /**
     * @brief Add a link between two nodes
     * @param node The node where to create link
     * @param link The other node to link with node
     * @return True if link successfully created
     */
    bool linkNodes(const size_t& node, const size_t& link) {
        auto n_node = manager->getNode(node);
        if (n_node == nullptr)
            return false;
        auto n_link = manager->getNode(link);
        if (n_link == nullptr)
            return false;
        return n_node->linkNode(n_link);
    }

    /**
     * @brief Get a node
     * @tparam T Node type
     * @return The node
     */
    template<class T>
    std::shared_ptr<T> getNode() {
        if (!std::is_base_of<driver::Node, T>::value)// only class derived from Node is allowed
            return nullptr;
        return manager->getDriver<T>();
    }

    /**
     * @brief Get the messenger
     * @return The messenger
     */
    std::shared_ptr<driver::Messenger> getMessenger() { return messenger; }

private:
    /// The driver manager
    std::shared_ptr<driver::Manager> manager = nullptr;
    /// The message manager
    std::shared_ptr<driver::Messenger> messenger = nullptr;
};

}// namespace obd::core
