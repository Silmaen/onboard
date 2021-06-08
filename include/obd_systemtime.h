/**
 * \author argawaen 
 * \date 08/06/2021
 *
 */

#pragma once
#include "obd_basedriver.h"
#include <ctime>

namespace obd::time {

class clock : public core::baseDriver {
public:
    explicit clock(core::system* p = nullptr) :
        baseDriver{p} {}

    /**
     * @brief retrieve the name of the driver
     * @return name of the driver
     */
    [[nodiscard]] String getName() const override { return F("SystemClock");}

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
private:
    /**
     * @brief display the curent date and time of the system
     */
    void printDate();
    /// base epoch time
    time_t epoch = 0;
    /// direct link to the net driver
    network::driver* net = nullptr;
    /// last timestamp
    uint64_t lastTs;
};

}// namespace obd::time
