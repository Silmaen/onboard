/**
 * @file BaseDriver.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Command.h"
#include "SystemInterface.h"


namespace obd::core {

class System_old;

/**
 * @brief Base class handling a Driver
 */
class BaseDriver : public SystemInterface {
public:
    BaseDriver()                  = delete;
    BaseDriver(const BaseDriver&) = delete;
    BaseDriver(BaseDriver&&)      = delete;
    BaseDriver& operator=(const BaseDriver&) = delete;
    BaseDriver& operator=(BaseDriver&&) = delete;
    /// Destructor
    virtual ~BaseDriver() = default;
    /**
     * @brief Constructor with parent
     * @param parent The parent system
     */
    explicit BaseDriver(System_old* parent = nullptr) :
        SystemInterface{parent} {}
    /**
     * @brief Initialize the driver
     * @return If well initialized
     */
    virtual bool init() {
        _initialized = isLinked();
        return _initialized;
    };
    /**
     * @brief Terminate the driver
     */
    virtual void end() {
        _initialized = false;
    };

    /**
     * @brief Check the initialization of the driver
     * @return The initialization state of the driver
     */
    [[nodiscard]] bool initialized()const{
        return _initialized;
    }
    /**
    * @brief Retrieve the driver's type
    * @return Driver's type as string
    */
    [[nodiscard]] OString getName() const;

    /**
     * @brief Print the driver infos
     */
    virtual void printInfo(){};
    /**
     * @brief Procedure called each frame
     * @param delta The delta from last update
     */
    virtual void update([[maybe_unused]] int64_t delta){};

    /**
     * @brief Try to treat the given command
     * @param cmd The command to treat
     * @return True if command successfully treated
     */
    virtual bool treatCommand([[maybe_unused]] const Command& cmd) {
        return false;
    }

    /**
     * @brief Display driver help on commands
     */
    virtual void printHelp(){};

    /**
     * @brief Load and apply parameters in the config file
     */
    virtual void loadConfigFile(){};

    /**
     * @brief Save the driver parameter in file
     */
    virtual void saveConfigFile() const {};

private:
    /// Initialization state of the driver
    bool _initialized = false;
};

}// namespace obd::core
