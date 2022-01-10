/**
 * @author Silmaen
 * @date 10/05/2021.
 */
#pragma once

#include <obd_drivermanager.h>
#include <obd_basedriver.h>
#include <obd_multistream.h>
#include <obd_status_led.h>
#include <obd_system_cmd.h>
#include <obd_dataserie.h>
#include <queue>

namespace obd::core {

/**
 * @brief Implementation of the system
 */
class System {
public:
    /**
     * @brief default constructor
     */
    System();

    /**
     * @brief Initialize the system
     */
    void init();

    /**
     * @brief Update the system
     */
    void update();

    /**
     * @brief Get the multi stream output
     * @return The outputs
     */
    Print* getPrint() {
        return &outputs;
    }

    /**
     * @brief Add a print to the list
     * @param p The new Print to add
     */
    void addPrint(Print* p) {
        outputs.addPrint(p);
    }

    /**
     * @brief Get the actual timestamp
     * @return The time stamp
     */
    [[nodiscard]] uint64_t getTimestamp() const {
        return timestamp;
    }

    /**
     * @brief Get the driver by its name and convert it to desired type
     * @tparam T The desired output type (must inherit from baseDriver)
     * @return The driver (nullptr if not exists or if template class does not inherit from baseDriver)
     */
    template<class T>
    std::shared_ptr<T> getDriver() {
        return drivers.getDriver<T>();
    }

    /**
     * @brief Print the kernel information
     */
    void printKernelInfo();

    /**
     * @brief Add a command to the list
     * @param cmd The command to add
     */
    void pushCommand(const command& cmd) {
        commands.push(cmd);
    }

    /**
     * @brief Return a direct pointer to the outputs list
     * @return The outputs list
     */
    MultiPrint* getOutput() {
        return &outputs;
    }

    /**
     * @brief Load and apply parameters from config files
     */
    void loadAllConfig();

    /**
     * @brief Save config in files
     */
    void saveAllConfig();

private:
    /**
     * @brief Print the system information
     */
    void printSystemInfo();

    /**
     * @brief Print the system help
     * @param[in] param The sub category for help
     */
    void printHelp(const String& param);

    /**
     * @brief display the statistics about timing
     */
    void printTimeStat();

    /**
     * @brief Treat the command queue
     */
    void treatCommands();

    /// List of output streams
    MultiPrint outputs;

    /// List of the drivers
    DriverManager drivers;

    /// Queue of the commands
    std::queue<command> commands;

    /// current timestamp
    uint64_t timestamp = 0;

    /// Data to collect statistics about time treatment
    data::Series<int64_t, 20> timeData;
};

}// namespace obd::core
