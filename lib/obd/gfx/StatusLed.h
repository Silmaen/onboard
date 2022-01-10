/**
 * @file StatusLed.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/BaseDriver.h"

namespace obd::gfx {

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
 * @note Do nothing in native platform
 */
class StatusLed : public core::BaseDriver {
public:
    /**
     * @brief Constructor with parent
     * @param parent The parent system
     */
    explicit StatusLed(core::System* parent) :
        BaseDriver(parent){};

    /**
     * @brief Initialize the driver
     * @return If init succeed
     */
    bool init() override;

    /**
     * @brief End the driver
     */
    void end() override{}

    /**
     * @brief Print the driver infos
     */
    void printInfo() override {}

    /**
     * @brief Procedure called each frame
     * @param delta The time delta from last update
     */
    void update(int64_t delta) override;

    /**
     * @brief Try to treat the given command
     * @param cmd The command to treat
     * @return True if the command has been treated
     */
    bool treatCommand(const core::Command& cmd) override;

    /**
     * @brief Display help on command
     */
    void printHelp() override;

    /**
     * @brief Load and apply parameters in the config file
     */
    void loadConfigFile() override {}

    /**
     * @brief Save the driver parameter in file
     */
    void saveConfigFile() const override {}

    /**
     * @brief Define the new state of the led
     * @param newState The new state
     */
    void setState(LedState newState = LedState::Off);

private:
    /**
     * @brief Print the current state of the LED
     */
    void printCurrentState();

    /**
     * @brief Callback function giving the LED fast blinking light according to the ledTimer
     * @return Led light value
     */
    [[nodiscard]] uint8_t fastBlinkCb() const;

    /**
     * @brief Callback function giving the LED two pulses light according to the ledTimer
     * @return Led light value
     */
    [[nodiscard]] uint8_t twoPulsesCb() const;

    /**
     * @brief Callback function giving the LED three pulses light according to the ledTimer
     * @return Led light value
     */
    [[nodiscard]] uint8_t threePulsesCb() const;

    /**
     * @brief Callback function giving the LED faster blinking light according to the ledTimer
     * @return Led light value
     */
    [[nodiscard]] uint8_t fasterBlinkCb() const;

    /// Current state of the led
    LedState ledState = LedState::Off;

    /// Chronometer for led
    uint64_t ledTime = 0;
};

}// namespace obd::gfx
