//
// Created by damien.lachouette on 10/05/2021.
//

#pragma once
#include <queue>
#include <obd_system_cmd.h>
#include "odb_multistream.h"

/**
 * @brief base namespace of the project
 */
namespace obd {
/**
 * @brief namespace for the base system
 */
namespace system {

/**
 * @brief implementation of the system
 */
class impl {
public:
    /**
     * @brief initialize the system
     */
    void init();
    /**
     * @brief update the system
     */
    void update();

private :
    /**
     * @brief print the kernel information
     */
    void printKernelInfo();

    /**
     * @brief print the system information
     */
    void printSystemInfo();

    /**
     * @brief print the network informations
     */
    void printNetworkInfo();

    /**
     * @brief treat the command queue
     */
    void treatCommands();

    /**
     * @brief list of output streams
     */
    MultiPrint outputs;
    /**
     * @brief queue of the commands
     */
    std::queue<command> commands;
};

}// namespace system

extern system::impl hardware;

} // namespace obd
