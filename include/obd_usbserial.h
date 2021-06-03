/**
 * \author argawaen 
 * \date 30/05/2021
 *
 */

#pragma once
#include <obd_basedriver.h>

namespace obd {
namespace core {

class UsbSerial : public baseDriver {
public:
    explicit UsbSerial(core::system *p = nullptr) : baseDriver(p) {};
    ~UsbSerial() = default;
    UsbSerial(const UsbSerial &) = default;
    UsbSerial(UsbSerial &&) = default;
    UsbSerial &operator=(const UsbSerial &) = default;
    UsbSerial &operator=(UsbSerial &&) = default;

    /**
     * @brief retrieve the name of the driver
     * @return name of the driver
     */
    const char *getName() const override{
        return "UsbSerial";
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

}// namespace core
}// namespace obd
