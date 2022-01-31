/**
 * @file Clock.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <memory>
#include <utility>
#include "core/driver/Node.h"
#include "fs/FileSystem.h"

/**
 * @brief Function of time manipulation
 */
namespace obd::time {

/// basic initialization of time zone
const OString TZ_Europe_Paris = "CET-1CEST,M3.5.0,M10.5.0/3";

/**
 * @brief Base driver for the clock
 */
class Clock : public core::driver::Node {
public:
    /**
     * @brief Constructor with parent system
     * @param parent The parent system
     */
    explicit Clock(std::shared_ptr<Messenger> parent) :
        Node{std::move(parent)}{}

    /**
     * @brief Initialize the driver
     */
    void init() override;

    /**
     * @brief Return the driver infos
     * @return The driver's infos.
     */
    [[nodiscard]] OString info()const override;

    /**
     * @brief Load and apply parameters in the config file
     */
    void loadConfig() override;

    /**
     * @brief Save the driver parameter in file
     */
    void saveConfig() const override;

    /**
     * @brief Formatting a given time
     * @param time The time to format
     * @return The formatted string
     */
    static OString formatTime(const time_t& time);

    /**
     * @brief Get the date as a string
     * @return The time as string
     */
    static OString getDateFormatted();

    /**
     * @brief Get the current time as posix time
     * @return The posix time
     */
    [[nodiscard]] static time_t getDate() ;

    /**
     * @brief Define the pool driver
     * @param pool The new pool driver
     */
    void setPoolServer(const OString& pool);

    /**
     * @brief Define the time zone
     * @param timeZone The new time zone
     */
    void setTimeZone(const OString& timeZone);

    /**
     * @brief Return the pool server
     * @return The pool server
     */
    [[nodiscard]] const OString& getPoolServer()const{return poolServerName;}

    /**
     * @brief Return the timezone
     * @return The timezone
     */
    [[nodiscard]] const OString & getTimeZone()const {return _timeZone;}

    /**
     * @brief Append some time to the chronometer
     * @param addedTime The time to add
     */
    void accelerateTime(uint64_t addedTime);

private:

    /**
     * @brief What to do before message treatment
     */
    void preTreatment() override;

    /**
     * @brief Configure the system time
     */
    void configTime();
    /**
     * @brief Send a message to this driver
     * @param message The Command message to send
     * @return True mean command caught.
     */
    bool pushCommand(const Message& message) override;
    /**
     * @brief Try to treat the given command
     * @param cmd The command to treat
     * @return True if the command has been treated
     */
    bool treatMessage(const Message& message) override;

    /// link to filesystem
    std::shared_ptr<fs::FileSystem> fileSystem = nullptr;
    /**
     * @brief check if the is a ready file system
     * @return
     */
    [[nodiscard]] bool checkFs() const;

    /// The pool server name to query
    OString poolServerName = "pool.ntp.org";

    /// Timezone configuration string
    OString _timeZone = TZ_Europe_Paris;

    /// Internal timestamp
    uint64_t timestamp = 0;

    /// Internal timer
    uint64_t chronometer = 0;
};

}// namespace obd::time
