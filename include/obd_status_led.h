/**
 * \author argawaen 
 * \date 30/05/2021
 *
 */

#pragma once
#include <obd_basedriver.h>

namespace obd::core {

/**
 * @brief States of the internal LED
 */
enum struct LedState {
    Off,        ///< ________  Led off
    Solid,      ///< --------  Led lit in a solid state
    Blink,      ///< ----____  Led is blinking with a 2s period
    FastBlink,  ///< --__--__  Led is blinking faster
    TwoPulse,   ///< -_-_____  Led do 2 pulses then wait
    ThreePulses,///< -_-_-___  Led do 3 pulses then wait
    FasterBlink,///< -_-_-_-_  Led is blinking very fast
};

/**
 * @brief Class to handle the Internal LED
 */
class StatusLed : public baseDriver {
public:
    explicit StatusLed(core::system* p = nullptr) :
        baseDriver(p){};
    ~StatusLed()                = default;
    StatusLed(const StatusLed&) = default;
    StatusLed(StatusLed&&)      = default;
    StatusLed& operator=(const StatusLed&) = default;
    StatusLed& operator=(StatusLed&&) = default;

    /**
     * @brief initialize the driver
     */
    void init() override;

    /**
     * @brief print the driver infos
     */
    void printInfo() override {}

    /**
     * @brief procedure called each frame
     */
    void update(uint64_t timestamp) override;

    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    bool treatCommand(const core::command& cmd) override;

    /**
     * @brief display help on command
     */
    void printHelp() override;

    /**
     * @brief get the name of the driver
     * @return driver name
     */
    [[nodiscard]] String getName() const override { return F("Status Led"); }

    /**
     * @brief define the new state of the led
     * @param st the new state
     */
    void setState(LedState st = LedState::Off);

private:
    /// current state of the led
    LedState ledState = LedState::Off;

    /// chronometer for led
    uint64_t ledTime = 0;
};

}// namespace obd::core
