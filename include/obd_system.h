//
// Created by damien.lachouette on 10/05/2021.
//

#pragma once
#include "odb_multistream.h"
#include <obd_system_cmd.h>
#include <obd_filesystem.h>
#include <obd_network.h>
#include <obd_status_led.h>
#include <queue>

/**
 * @brief base namespace of the project
 */
namespace obd {
/**
 * @brief namespace for the base system
 */
namespace core {

/**
 * @brief implementation of the system
 */
class system {
public:
    system():fs(this), net(this), led(this) {}
    ~system() = default;
    system(const system &) = default;
    system(system &&) = default;
    system &operator=(const system &) = default;
    system &operator=(system &&) = default;
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

    /**
     * @brief get
     * @return
     */
    Print* getPrint(){
        return &outputs;
    }

    /**
     * @brief get the actual timestamp
     * @return the time stamp
     */
    uint64_t getTimestamp()const{
        return timestamp;
    }
private:
    /**
     * @brief print the kernel information
     */
    void printKernelInfo();

    /**
     * @brief print the system information
     */
    void printSystemInfo();

    /**
     * @brief treat the command queue
     */
    void treatCommands();

    /**
     * @brief Update the state of the LED
     */
    void updateLedState();

    /// list of output streams
    MultiPrint outputs;

    /// filesystem driver
    filesystem::driver fs;

    /// network driver
    network::driver net;

    /// status led driver
    StatusLed led;

    /// queue of the commands
    std::queue<command> commands;

    /// current timestamp
    uint64_t timestamp = 0;

    /// current state of the led
    LedState ledState = LedState::Off;

    /// chronometer for led
    uint64_t ledTime = 0;
};

}// namespace core
}// namespace obd
