/**
 * @file Clock.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <memory>
#include "core/BaseDriver.h"
#include "fs/FileSystem.h"

/**
 * @brief Function of time manipulation
 */
namespace obd::time {

/// basic initialization of time zone
const std::string TZ_Europe_Paris = "CET-1CEST,M3.5.0,M10.5.0/3";

/**
 * @brief Base driver for the clock
 */
class Clock : public core::BaseDriver {
public:
    /**
     * @brief Constructor with parent system
     * @param parent The parent system
     */
    explicit Clock(core::System* parent) :
        BaseDriver{parent}{}

    /**
     * @brief Initialize the driver
     * @return True if everything is ok
     */
    bool init() override;

    /**
     * @brief Print the driver infos
     */
    void printInfo() override;

    /**
     * @brief Procedure called each frame
     * @param delta The current timestamp of the device
     */
    void update(int64_t delta) override;

    /**
     * @brief Try to treat the given command
     * @param cmd The command to treat
     * @return True if the command has been treated
     */
    bool treatCommand(const core::Command& cmd) override;

    /**
     * @brief Display driver help on commands
     */
    void printHelp() override;

    /**
     * @brief Load and apply parameters in the config file
     */
    void loadConfigFile() override;

    /**
     * @brief Save the driver parameter in file
     */
    void saveConfigFile() const override;

    /**
     * @brief Formatting a given time
     * @param time The time to format
     * @return The formatted string
     */
    static std::string formatTime(const time_t& time);

    /**
     * @brief Get the date as a string
     * @return The time as string
     */
    static std::string getDateFormatted();

    /**
     * @brief Get the current time as posix time
     * @return The posix time
     */
    [[nodiscard]] static time_t getDate() ;

    /**
     * @brief Define the pool driver
     * @param pool The new pool driver
     */
    void setPoolServer(const std::string& pool);

    /**
     * @brief Define the time zone
     * @param timeZone The new time zone
     */
    void setTimeZone(const std::string& timeZone);

    /**
     * @brief Return the pool server
     * @return The pool server
     */
    [[nodiscard]] const std::string& getPoolServer()const{return poolServerName;}

    /**
     * @brief Return the timezone
     * @return The timezone
     */
    [[nodiscard]] const std::string & getTimeZone()const {return _timeZone;}
private:
    /**
     * @brief Display the current date and time of the system
     */
    void printDate();

    /**
     * @brief Configure the system time
     */
    void configTime();

    /// link to filesystem
    std::shared_ptr<fs::FileSystem> fileSystem = nullptr;

    /// The pool server name to query
    std::string poolServerName = "pool.ntp.org";

    /// Timezone configuration string
    std::string _timeZone = TZ_Europe_Paris;

    /// Internal timer
    uint64_t chronometer = 0;
};

}// namespace obd::time
