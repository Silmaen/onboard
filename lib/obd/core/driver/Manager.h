/**
 * @file Manager.h
 * @author argawaen
 * @date 14/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Node.h"
#include <algorithm>
#include <memory>

namespace obd::core::driver {

/**
 * @brief Class to manage all the drivers
 */
class Manager:public base::Object {
public:/// Pointer to a driver node
    using BaseNodeType = Node;
    /// Node name's type
    using NameType = OString;
    /// Pointer to a driver node
    using NodeType = std::shared_ptr<BaseNodeType>;
    /// List of pointer to driver node
    using NodeList = std::vector<NodeType>;
    /// Iterator type
    using iterator = NodeList::iterator;
    /// Constant iterator type
    using const_iterator = NodeList::const_iterator;

    /**
     * @brief Default constructor.
     */
    Manager() = default;

    /**
     * @brief initialize
     */
    void init()override;

    /**
     * @brief Get a pointer to the node given its name
     * @param nodeName The name of the node
     * @return The node.
     */
    [[nodiscard]] NodeType getNode(const NameType& nodeName);

    /**
     * @brief Get a pointer to the node given its name
     * @param nodeHash The name of the node
     * @return The node.
     */
    [[nodiscard]] NodeType getNode(const size_t& nodeHash);

    /**
     * @brief Get the driver of the given type
     * @tparam T The driver's type to search
     * @return A pointer to the driver, return nullptr for any error
     */
    template<class T>
    std::shared_ptr<T> getDriver() {
        if (!std::is_base_of<BaseNodeType, T>::value)// only class derived from Node is allowed
            return nullptr;
        auto search = std::find_if(begin(), end(),
                                   [](const NodeType& driver) {
                                       return typeid(T).hash_code() == driver->type();
                                   });
        if (search == end()) {
            return nullptr;
        }
        return std::dynamic_pointer_cast<T>(*search);
    }

    /**
     * @brief Get begin iterator of the driver list
     * @return Begin iterator of the driver list
     */
    iterator begin() { return nodes.begin(); }

    /**
     * @brief Get end iterator of the driver list
     * @return End iterator of the driver list
     */
    iterator end() { return nodes.end(); }

    /**
     * @brief Get begin iterator of the driver list
     * @return Begin iterator of the driver list
     */
    [[nodiscard]] const_iterator begin() const { return nodes.begin(); }

    /**
     * @brief Get end iterator of the driver list
     * @return End iterator of the driver list
     */
    [[nodiscard]] const_iterator end() const { return nodes.end(); }
    /**
     * @brief Get begin iterator of the driver list
     * @return Begin iterator of the driver list
     */
    [[nodiscard]] const_iterator cbegin() const { return nodes.cbegin(); }

    /**
     * @brief Get end iterator of the driver list
     * @return End iterator of the driver list
     */
    [[nodiscard]] const_iterator cend() const { return nodes.cend(); }

    /**
     * @brief Get the size of the node's list.
     * @return Node list size.
     */
    [[nodiscard]] size_t size() const { return nodes.size(); }

    /**
     * @brief Add the given node to the list
     * @param node The node to add
     * @return True if the node truly added
     */
    bool addNode(const NodeType& node);

    void update()override;

private:
    /// Node’s list
    NodeList nodes;
};

}// namespace obd::core::driver
