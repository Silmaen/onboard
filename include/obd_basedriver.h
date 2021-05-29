/**
 * \author argawaen 
 * \date 29/05/2021
 *
 */

#pragma once
#include "classfwd.h"
#include "obd_system_cmd.h"
#include <Print.h>

namespace obd {
namespace core {

/**
 * @brief base implementation of a system driver
 */
class baseDriver {
public:
    /**
     * @brief constructor with parent
     * @param p the parent system
     */
    explicit baseDriver(system *p) : parent{p} {}

    /**
     * @brief attach a new parent to this driver
     * @param p the parent
     */
    void attachParent(system *p) {
        parent = p;
    }

    /**
     * @brief get the parent system
     * @return the parent
     */
    system *getParent() {
        return parent;
    }

    /**
     * @brief get the outputs from the parent system
     * @return the output
     */
    Print *getParentPrint();

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
     */
    virtual void update() = 0;

    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    virtual bool treatCommand(const core::command&  /*cmd*/){
        return false;
    }
private:
    /// pointer to the parent system
    system *parent = nullptr;
};

}// namespace core
}// namespace obd
