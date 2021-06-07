/**
 * @author Silmaen
 * @date 30/05/2021
 */

#pragma once
#include <obd_basedriver.h>

namespace obd::core {

/**
 * @brief class to handle communication through USB
 */
class UsbSerial : public baseDriver {
public:
    /**
     * @brief Constructor with parent
     * @param p the parent system
     */
    explicit UsbSerial(core::system* p = nullptr) :
        baseDriver(p){};

    /**
     * @brief retrieve the name of the driver
     * @return name of the driver
     */
    [[nodiscard]] String getName() const override {
        return F("UsbSerial");
    }

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
     */
    void update(uint64_t timestamp) override;

    /**
     * @brief display driver help on commands
     */
    void printHelp() override {}

private:
};

}// namespace obd::core
