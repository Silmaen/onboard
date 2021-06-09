/**
 * @author Silmaen
 * @date 30/05/2021
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
    /**
     * @brief Constructor with parent
     * @param p the parent system
     */
    explicit StatusLed(core::system* p = nullptr) :
        baseDriver(p){};

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
     * @param delta the time delta from last update
     */
    void update(int64_t delta) override;

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
    [[nodiscard]] String getName() const override { return F("StatusLed"); }

    /**
     * @brief load and apply parameters in the config file
     */
    void loadConfigFile() override {}

    /**
     * @brief save the driver parameter in file
     */
    void saveConfigFile() const override {}

    /**
     * @brief define the new state of the led
     * @param st the new state
     */
    void setState(LedState st = LedState::Off);

private:
    /**
     * @brief print the current state of the led
     */
    void printCurrentState();

    /// current state of the led
    LedState ledState = LedState::Off;

    /// chronometer for led
    uint64_t ledTime = 0;
};

}// namespace obd::core
