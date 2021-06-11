/**
 * \author argawaen 
 * \date 08/06/2021
 *
 */

#pragma once
#include "obd_basedriver.h"
#include <TZ.h>

namespace obd::time {

/**
 * @brief get time for call back function
 * @return current time
 */
time_t timeCb();

class clock : public core::baseDriver {
public:
    explicit clock(core::system* p = nullptr) :
        baseDriver{p} {}

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
     * @brief get the date as a string
     * @return the time as string
     */
    String getDateFormatted();

    /**
     * @brief get the current time as posix time
     * @return the posix time
     */
    [[nodiscard]] time_t getDate() const;
private:
    /**
     * @brief display the current date and time of the system
     */
    void printDate();

    /// the pool server name to query
    String poolServerName = "pool.ntp.org";
};

}// namespace obd::time
