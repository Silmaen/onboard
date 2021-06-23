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

namespace obd::core {

/**
 * @brief implementation of the system
 */
class system {
public:
    /**
     * @brief default constructor
     */
    system();

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
    std::shared_ptr<baseDriver> getDriver(const String& name);

    /**
     * @brief get the driver by its name and convert it to desired type
     * @tparam T the desired output type (must inherit from baseDriver)
     * @param name the driver name
     * @return the driver (nullptr if not exists or if template class does not inherit from baseDriver)
     */
    template<class T>
    std::shared_ptr<T> getDriverAs(const String& name) {
        if (!std::is_base_of<baseDriver, T>::value)
            return nullptr;
        std::shared_ptr<baseDriver> a = getDriver(name);
        if (a == nullptr)
            return nullptr;
        return std::static_pointer_cast<T>(a);
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

    /**
     * @brief load and apply parameters from config files
     */
    void loadAllConfig();

    /**
     * @brief save config in files
     */
    void saveAllConfig();

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
    std::vector<std::shared_ptr<baseDriver>> drivers;

    /// queue of the commands
    std::queue<command> commands;

    /// current timestamp
    uint64_t timestamp = 0;
};

}// namespace obd::core
