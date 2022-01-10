/**
 * @author Silmaen
 * @date 29/05/2021
 */
#pragma once

#include "obd_systeminterface.h"

namespace obd::core {


/**
 * @brief Base implementation of a system driver
 */
class BaseDriver : public SystemInterface {
public:
    /**
     * @brief Constructor with parent
     * @param p The parent system
     */
    explicit BaseDriver(System* p = nullptr) :
        SystemInterface{p} {}

    /**
     * @brief Retrieve the type of the driver
     * @return Type of the driver as string
     */
    [[nodiscard]] String getName() const;

    /**
     * @brief Initialize the driver
     */
    virtual void init() = 0;

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
    virtual bool treatCommand([[maybe_unused]] const command& cmd) {
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
};

}// namespace obd::core
