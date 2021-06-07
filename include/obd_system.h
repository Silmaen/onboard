/**
 * @author Silmaen
 * @date 10/05/2021.
 */

#pragma once
#include <obd_basedriver.h>
#include <obd_multistream.h>
#include <obd_status_led.h>
#include <obd_system_cmd.h>
#include <queue>

/**
 * @namespace obd
 * @brief base namespace of the project
 */
/**
 * @namespace obd::core
 * @brief namespace for the base system
 */
namespace obd::core {

/**
 * @brief implementation of the system
 */
class system {
public:
    system();
    ~system()             = default;
    system(const system&) = default;
    system(system&&)      = default;
    system& operator=(const system&) = default;
    system& operator=(system&&) = default;
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
    Print* getPrint() {
        return &outputs;
    }

    /**
     * @brief add a print to the list
     * @param p the new Print to add
     */
    void addPrint(Print* p) {
        outputs.addPrint(p);
    }

    /**
     * @brief get the actual timestamp
     * @return the time stamp
     */
    [[nodiscard]] uint64_t getTimestamp() const {
        return timestamp;
    }

    /**
     * @brief get a driver by its name
     * @param name the name of the driver
     * @return the driver (nullptr if not exists)
     */
    baseDriver* getDriver(const String& name);

    /**
     * @brief get the driver by its name and convert it to desired type
     * @tparam T the desired output type (must inherit from baseDriver)
     * @param name the driver name
     * @return the driver (nullptr if not exists or if template class does not inherit from baseDriver)
     */
    template<class T>
    T* getDriverAs(const String& name) {
        if (!std::is_base_of<baseDriver, T>::value)
            return nullptr;
        baseDriver* a = getDriver(name);
        if (a == nullptr)
            return nullptr;
        return static_cast<T*>(a);
    }

    /**
     * @brief print the kernel information
     */
    void printKernelInfo();

    /**
     * @brief add a command to the list
     * @param cmd the command to add
     */
    void pushCommand(const command& cmd) {
        commands.push(cmd);
    }

    /**
     * @brief return a direct pointer to the outputs list
     * @return the outputs list
     */
    MultiPrint* getOutput() {
        return &outputs;
    }

private:
    /**
     * @brief print the system information
     */
    void printSystemInfo();

    /**
     * @brief print the system help
     * @param[in] param the sub category for help
     */
    void printHelp(const String& param);

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

}// namespace obd::core
