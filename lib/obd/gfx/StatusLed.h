/**
 * @file StatusLed.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/driver/Node.h"

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
class StatusLed : public core::driver::Node {
public:
    /**
     * @brief Constructor with parent
     * @param parent The parent system
     */
    explicit StatusLed(std::shared_ptr<Messenger> parent) :
        Node(parent){};

    /**
     * @brief Initialize the driver
     */
    void init() override;

    /**
     * @brief Define the new state of the led
     * @param newState The new state
     */
    void setState(LedState newState = LedState::Off);

    /**
     * @brief Get current LED state
     * @return The LED state.
     */
    [[nodiscard]] const LedState& state()const{return ledState;}

    /**
     * @brief Append some time to the chronometer
     * @param addedTime The time to add
     */
    void accelerateTime(uint64_t addedTime);
private:
    /**
     * @brief Print the current state of the LED
     */
    void printCurrentState();

    /**
     * @brief Procedure called each frame
     * @param delta The time delta from last update
     */
    void preTreatment() override;

    /**
     * @brief Try to treat the given command
     * @param cmd The command to treat
     * @return True if the command has been treated
     */
    bool treatMessage(const Message& cmd) override;

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
    /**
     * @brief Send a message to this driver
     * @param message The Command message to send
     * @return True mean command caught.
     */
    bool pushCommand(const Message& message) override;

    /// Current state of the led
    LedState ledState = LedState::Off;

    /// Internal timestamp
    uint64_t timestamp = 0;

    /// Chronometer for led
    uint64_t ledTime = 0;
};

}// namespace obd::gfx
