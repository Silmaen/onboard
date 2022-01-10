/**
 * @file Console.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/BaseDriver.h"

namespace obd::com {

/**
 * @brief Class to handle console communication
 *
 * @note on Arduino Platforms it pass through USB Serial
 */
class Console : public core::BaseDriver {
public:
    /**
     * @brief Constructor with parent
     * @param parent The parent system
     */
    explicit Console(core::System* parent) :
        BaseDriver(parent) {}

    /**
     * @brief Initialize the driver
     * @return If well initialized
     */
    bool init() override;

    /**
     * @brief End the driver
     */
    void end() override{}
    /**
     * @brief Print the driver infos
     */
    void printInfo() override;

    /**
     * @brief Procedure called each frame
     * @param delta The time delta from last update
     */
    void update(int64_t delta) override;

    /**
     * @brief Display driver help on commands
     */
    void printHelp() override {}

    /**
     * @brief Load and apply parameters in the config file
     */
    void loadConfigFile() override {}

    /**
     * @brief Save the driver parameter in file
     */
    void saveConfigFile() const override {}

private:
    /// Pointer to the output of this
    Output* _output = nullptr;
};

}// namespace obd::com
