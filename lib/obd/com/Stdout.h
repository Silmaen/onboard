/**
 * @file Stdout.h
 * @author argawaen
 * @date 19/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "com/base/ComNode.h"

namespace obd::com {

/**
 * @brief Standard output
 */
class Stdout : public base::ComNode {
public:
    /**
     * @brief Default constructor.
     * @param parent The link to the messenger system
     */
    explicit Stdout(std::shared_ptr<Messenger> parent):ComNode{std::move(parent)}{}

    void init()override;
private:

    bool available()override;

    OString readLine()override;

    void writeLine(const OString& line)override;
};

}// namespace obd::com
