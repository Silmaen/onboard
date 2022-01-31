/**
 * @author Silmaen
 * @date 30/05/2021
 */
#pragma once

#include <obd_basedriver.h>

namespace obd::network {

/**
 * @brief class to handle communication through USB
 */
class UsbSerial : public core::BaseDriver {
public:
    /**
     * @brief Constructor with parent
     * @param p the parent system
     */
    explicit UsbSerial(core::System* p = nullptr) :
        BaseDriver(p){};

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
     * @param delta the time delta from last update
     */
    void update(int64_t delta) override;

    /**
     * @brief display driver help on commands
     */
    void printHelp() override {}

    /**
     * @brief load and apply parameters in the config file
     */
    void loadConfigFile() override {}

    /**
     * @brief save the driver parameter in file
     */
    void saveConfigFile() const override {}

private:
};

}// namespace obd::network
