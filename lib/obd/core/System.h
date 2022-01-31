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
class System : public base::Object{
public:
    System(const System&) =delete;
    System(System&&) =delete;
    System& operator=(const System&) =delete;
    System& operator=(System&&) =delete;
    /**
     * @brief Default constructor.
     */
    System();
    /**
     * @brief Destructor.
     */
    ~System() override;

    /**
     * @brief Initialisation
     */
    void init()override;

    /**
     * @brief Actualization frame
     */
    void update()override;

    /**
     * @brief Do a full system check
     * @return True if everything ok
     */
    [[nodiscard]] bool check()override;

    /**
     * @brief Add a node
     * @tparam T The node's type to add
     * @return True if the node truly added
     */
    template<class T>
    bool addNode(){
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
    bool linkNodes(const size_t& node, const size_t& link);

    /**
     * @brief Get a node
     * @tparam T Node type
     * @return The node
     */
    template<class T>
    std::shared_ptr<T> getNode(){
        if (!std::is_base_of<driver::Node, T>::value)// only class derived from Node is allowed
            return nullptr;
        return manager->getDriver<T>();
    }

    /**
     * @brief Get the messenger
     * @return The messenger
     */
    std::shared_ptr<driver::Messenger> getMessenger(){return messenger;}
private:
    /// The driver manager
    std::shared_ptr<driver::Manager> manager=nullptr;
    /// The message manager
    std::shared_ptr<driver::Messenger> messenger= nullptr;
};

}// namespace obd::core
