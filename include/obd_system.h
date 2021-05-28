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
 * @brief States of the internal LED
 */
enum struct LedState {
    Off,   ///< Led off
    Solid, ///< Led lit in a solid state
    Blink, ///< Led is blinking with a 2s period
};

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

    /**
     * @brief define the new state of the LED
     * @param state the new state
     */
    void ledSetState(LedState state = LedState::Off);
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
    static void printNetworkInfo();

    /**
     * @brief treat the command queue
     */
    void treatCommands();

    /**
     * @brief Update the state of the LED
     */
    void updateLedState();

    /**
     * @brief list of output streams
     */
    MultiPrint outputs;

    /**
     * @brief queue of the commands
     */
    std::queue<command> commands;

    /**
     * @brief current timestamp
     */
    uint64_t timestamp = 0;

    /**
     * @brief current sztate of the led
     */
    LedState ledState= LedState::Off;
    uint64_t ledTime;
    bool ledVal;

};

}// namespace system

extern system::impl hardware;

} // namespace obd
