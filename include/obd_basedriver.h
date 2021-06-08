/**
 * @author Silmaen
 * @date 29/05/2021
 */

#pragma once
#include "classfwd.h"
#include "obd_system_cmd.h"
#include <Print.h>

namespace obd::core {

/**
 * @brief base implementation of a system driver
 */
class baseDriver {
public:
    /**
     * @brief constructor with parent
     * @param p the parent system
     */
    explicit baseDriver(system* p = nullptr) :
        parent{p} {}

    /**
     * @brief attach a new parent to this driver
     * @param p the parent
     */
    virtual void attachParent(system* p) {
        parent = p;
    }

    /**
     * @brief get the parent system
     * @return the parent
     */
    system* getParent() const {
        return parent;
    }

    /**
     * @brief get the outputs from the parent system
     * @return the output
     */
    Print* getParentPrint();

    /**
     * @brief retrieve the name of the driver
     * @return name of the driver
     */
    [[nodiscard]] virtual String getName() const = 0;

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
     * @param timestamp the current timestamp of the device
     */
    virtual void update(uint64_t timestamp) = 0;

    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    virtual bool treatCommand([[maybe_unused]] const core::command& cmd) {
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

private:
    /// pointer to the parent system
    system* parent = nullptr;
};

}// namespace obd::core
