/**
 * @author Silmaen
 * @date 29/05/2021
 */
#pragma once

#include "obd_systeminterface.h"

namespace obd::core {


/**
 * @brief base implementation of a system driver
 */
class BaseDriver : public SystemInterface {
public:
    /**
     * @brief constructor with parent
     * @param p the parent system
     */
    explicit BaseDriver(System* p = nullptr) :
        SystemInterface{p} {}

    /**
     * @brief retrieve the type of the driver
     * @return type of the driver as string
     */
    [[nodiscard]] String getName() const;

    /**
     * @brief initialize the driver
     */
    virtual void init() = 0;

    /**
     * @brief print the driver infos
     */
    virtual void printInfo() = 0;

    /**
     * @brief procedure called each frame
     * @param delta the delta from last update
     */
    virtual void update(int64_t delta) = 0;

    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    virtual bool treatCommand([[maybe_unused]] const command& cmd) {
        return false;
    }

    /**
     * @brief display driver help on commands
     */
    virtual void printHelp() = 0;

    /**
     * @brief load and apply parameters in the config file
     */
    virtual void loadConfigFile() = 0;

    /**
     * @brief save the driver parameter in file
     */
    virtual void saveConfigFile() const = 0;
};

}// namespace obd::core
