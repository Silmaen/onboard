/**
 * @file ComNode.h
 * @author argawaen
 * @date 19/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/driver/Node.h"

#include <utility>
namespace obd::com::base {

/**
 * @brief Base node used
 */
class ComNode : public core::driver::Node {
public:
    /**
     * @brief Default constructor.
     * @param parent The parent system
     */
    explicit ComNode(std::shared_ptr<Messenger> parent) :
        Node{std::move(parent)} {}

    /**
     * @brief Get the node's category
     * @return Node's category
     */
    [[nodiscard]] Category category() const final { return Category::Communicator; }

private:
    /**
     * @brief What to do before message treatment
     */
    void preTreatment() final;

    /**
     * @brief What to do after message treatment
     */
    void postTreatment() final;

    /**
     * @brief Treat the given message
     * @param message The message to treat
     * @return True if message treated
     */
    bool treatMessage(const Message& message);

    void listen();

    void talk();

    virtual bool available(){return false;}

    virtual OString readLine(){return {};}

    virtual void writeLine([[maybe_unused]] const OString& line){}
};
}// namespace obd::com::base
