/**
 * @file System.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "data/Series.h"
#include "BaseDriver.h"
#include "DriverManager.h"
#include "com/MultiOutput.h"
#include <cstdint>
#include <memory>
#include <queue>

namespace obd::core {

/**
 * @brief Base system
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
    void pushCommand(const Command& cmd) {
        commands.push(cmd);
    }

    /**
     * @brief Return a direct pointer to the outputs list
     * @return The outputs list
     */
    com::MultiOutput* getOutput() {
        return &outputs;
    }

    /**
     * @brief Add a print to the list
     * @param p The new Print to add
     */
    void addOutput(com::Output* p) {
        outputs.addOutput(p);
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
    void printHelp(const std::string& param);

    /**
     * @brief Display the statistics about timing
     */
    void printTimeStat();

    /**
     * @brief Treat the command queue
     */
    void treatCommands();

    /// List of output streams
    com::MultiOutput outputs;

    /// List of the drivers
    DriverManager drivers;

    /// Queue of the commands
    std::queue<Command> commands;

    /// current timestamp
    uint64_t timestamp = 0;

    /// Data to collect statistics about time treatment
    data::Series<int64_t, 20> timeData;
};

}// namespace obd::core
