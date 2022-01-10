/**
 * @file BaseDriver.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "SystemInterface.h"
#include "Command.h"


namespace obd::core {

class System;

/**
 * @brief Base class handling a Driver
 */
class BaseDriver: public SystemInterface {
public:
    /**
     * @brief Constructor with parent
     * @param parent The parent system
     */
    explicit BaseDriver(System* parent = nullptr) :
        SystemInterface{parent} {}
    /**
     * @brief Initialize the driver
     * @return If well initialized
     */
    virtual bool init() = 0;
    /**
     * @brief Terminate the driver
     */
    virtual void end() = 0;
    /**
    * @brief Retrieve the driver's type
    * @return Driver's type as string
    */
    [[nodiscard]] std::string getName() const;

    /**
 * @brief Print the driver infos
 */
    virtual void printInfo() = 0;
    /**
     * @brief Procedure called each frame
     * @param delta The delta from last update
     */
    virtual void update(int64_t delta) = 0;

    /**
     * @brief Try to treat the given command
     * @param cmd The command to treat
     * @return True if the command has been treated
     */
    virtual bool treatCommand([[maybe_unused]] const Command& cmd) {
        return false;
    }

    /**
     * @brief Display driver help on commands
     */
    virtual void printHelp() = 0;

    /**
     * @brief Load and apply parameters in the config file
     */
    virtual void loadConfigFile() = 0;

    /**
     * @brief Save the driver parameter in file
     */
    virtual void saveConfigFile() const = 0;

private:
};

}// namespace obd::core
