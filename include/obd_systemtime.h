/**
 * @author Silmaen
 * @date 08/06/2021
 */
#pragma once

#include "obd_basedriver.h"
#include <TZ.h>

namespace obd::time {

/**
 * @brief base driver for the clock
 */
class clock : public core::baseDriver {
public:
    /**
     * @brief constructor with parent system
     * @param p the parent system
     */
    explicit clock(core::system* p = nullptr) :
        baseDriver{p}, timeZone{TZ_Europe_Paris} {}

    /**
     * @brief retrieve the name of the driver
     * @return name of the driver
     */
    [[nodiscard]] String getName() const override { return F("SystemClock"); }

    /**
     * @brief initialize the driver
     */
    void init() override;

    /**
     * @brief print the driver infos
     */
    void printInfo() override;

    /**
     * @brief procedure called each frame
     * @param delta the current timestamp of the device
     */
    void update(int64_t delta) override;

    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    bool treatCommand(const core::command& cmd) override;

    /**
     * @brief display driver help on commands
     */
    void printHelp() override;

    /**
     * @brief load and apply parameters in the config file
     */
    void loadConfigFile() override;

    /**
     * @brief save the driver parameter in file
     */
    void saveConfigFile() const override;

    /**
     * @brief formatting a given time
     * @param time the time to format
     * @return the formatted string
     */
    static String formatTime(const time_t& time);

    /**
     * @brief get the date as a string
     * @return the time as string
     */
    static String getDateFormatted();

    /**
     * @brief get the current time as posix time
     * @return the posix time
     */
    [[nodiscard]] static time_t getDate() ;

    /**
     * @brief define the pool driver
     * @param pool the new pool driver
     */
    void setPoolServer(const String& pool);

    /**
     * @brief define the time zone
     * @param tz the new time zone
     */
    void setTimeZone(const String& tz);
private:
    /**
     * @brief display the current date and time of the system
     */
    void printDate();

    /// link to filesystem
    std::shared_ptr<filesystem::fs_driver> fs = nullptr;

    /// the pool server name to query
    String poolServerName = "pool.ntp.org";

    /// Timezone configuration string
    String timeZone;

    /// internal timer
    uint64_t chronometer = 0;
};

}// namespace obd::time
