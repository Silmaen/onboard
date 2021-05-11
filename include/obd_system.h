//
// Created by damien.lachouette on 10/05/2021.
//

#pragma once
#include <Arduino.h>
#include <queue>
#include <obd_system_cmd.h>

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
     * @brief print the system information in the given device
     * @param output the device onto write
     */
    static void printSystemInfo(Stream &output);

    /**
     * @brief treat the command queue
     */
    void treatCommands();

    /**
     * @brief queue of the commands
     */
    std::queue<command> commands;
};

}// namespace system

extern system::impl hardware;

} // namespace obd
