//
// Created by damien.lachouette on 10/05/2021.
//

#pragma once
#include <odb_multistream.h>
#include <obd_system_cmd.h>
#include <obd_basedriver.h>
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
    system();
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
     * @brief get
     * @return
     */
    Print* getPrint(){
        return &outputs;
    }

    /**
     * @brief add a print to the list
     * @param p the new Print to add
     */
    void addPrint(Print* p){
        outputs.addPrint(p);
    }

    /**
     * @brief get the actual timestamp
     * @return the time stamp
     */
    uint64_t getTimestamp()const{
        return timestamp;
    }

    /**
     * @brief get a driver by its id in the list
     * @param driver_id the id of the driver
     * @return the driver
     */
    baseDriver* getDriver(size_t driver_id);

    /**
     * @brief get a driver by its name
     * @param name the name of the driver
     * @return the driver (nullptr if not exists)
     */
    baseDriver* getDriver(const char* name);

    /**
     * @brief print the kernel information
     */
    void printKernelInfo();

    /**
     * @brief add a command to the list
     * @param cmd the command to add
     */
    void pushCommand(const command& cmd){
        commands.push(cmd);
    }
private:

    /**
     * @brief print the system information
     */
    void printSystemInfo();

    /**
     * @brief treat the command queue
     */
    void treatCommands();

    /// list of output streams
    MultiPrint outputs;

    /// list of the drivers
    std::vector<baseDriver*> drivers;

    /// queue of the commands
    std::queue<command> commands;

    /// current timestamp
    uint64_t timestamp = 0;
};

}// namespace core
}// namespace obd
