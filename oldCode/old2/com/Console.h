/**
 * @file Console.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/BaseDriver.h"
#include "ConsoleOutput.h"

namespace obd::com {

/**
 * @brief Class to handle console communication
 *
 * @note On Arduino Platforms it pass through USB Serial
 */
class Console : public core::BaseDriver {
public:
    /**
     * @brief Constructor with parent
     * @param parent The parent system
     */
    explicit Console(core::System_old* parent) :
        BaseDriver(parent) {}

    /**
     * @brief Initialize the driver
     * @return If well initialized
     */
    bool init() override;

    /**
     * @brief Print the driver infos
     */
    void printInfo() override;

    /**
     * @brief Procedure called each frame
     * @param delta The time delta from last update
     */
    void update(int64_t delta) override;
private:
    /// Pointer to the output of this
    std::shared_ptr<ConsoleOutput> _output = nullptr;
};

}// namespace obd::com
