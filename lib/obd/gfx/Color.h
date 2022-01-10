/**
 * @file Color.h
 * @author argawaen
 * @date 31/12/2021
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <cstdint>

/**
 * @brief Namespace gathering display functions
 */
namespace obd::gfx {

/**
 * @brief A simple struct to handle color data
 */
struct Color {
    /// red channel
    uint8_t red = 0;
    /// green channel
    uint8_t green = 0;
    /// blue channel
    uint8_t blue = 0;
    /**
   * @brief Convert this color into rgb565 format.
   * @return The formatted color code.
   */
    [[nodiscard]] uint16_t toRGB565() const {
        return (red5()) << 11U | ((green6()) & 0x3f) << 5U | ((blue5()) & 0x1f);
    }
    /**
   * @brief Convert this color into rgb332 format.
   * @return The formatted color code.
   */
    [[nodiscard]] uint8_t toRGB332() const {
        return (red3()) << 5U | ((green3()) & 0b111) << 2U | ((blue2()) & 0b11);
    }

    /**
   * @brief Convert the red channel into rgb565 format.
   * @return The formatted red channel.
   */
    [[nodiscard]] uint8_t red5() const { return (red * 0x1f) / 255; }
    /**
   * @brief Convert the green channel into rgb565 format.
   * @return The formatted green channel.
   */
    [[nodiscard]] uint8_t green6() const { return (green * 0x3f) / 255; }
    /**
   * @brief Convert the blue channel into rgb565 format.
   * @return The formatted blue channel.
   */
    [[nodiscard]] uint8_t blue5() const { return (blue * 0x1f) / 255; }
    /**
   * @brief Convert the red channel into rgb332 format.
   * @return The formatted red channel.
   */
    [[nodiscard]] uint8_t red3() const { return (red * 0b111) / 255; }
    /**
   * @brief Convert the green channel into rgb332 format.
   * @return The formatted green channel.
   */
    [[nodiscard]] uint8_t green3() const { return (green * 0b111) / 255; }
    /**
   * @brief Convert the blue channel into rgb332 format.
   * @return The formatted blue channel.
   */
    [[nodiscard]] uint8_t blue2() const { return (blue * 0b11) / 255; }
};

/// Predefined color red
constexpr Color red = {255, 0, 0};
/// Predefined color light red
constexpr Color lightLed = {255, 127, 127};
/// Predefined color yellow
constexpr Color yellow = {255, 255, 0};
/// Predefined color light yellow
constexpr Color lightYellow = {255, 255, 127};
/// Predefined green
constexpr Color green = {0, 255, 0};
/// Predefined light green
constexpr Color lightGreen = {127, 255, 127};
/// Predefined color cyan
constexpr Color cyan = {0, 255, 255};
/// Predefined color light cyan
constexpr Color lightCyan = {127, 255, 255};
/// Predefined color blue
constexpr Color blue = {0, 0, 255};
/// Predefined color light blue
constexpr Color lightBlue = {127, 127, 255};
/// Predefined color magenta
constexpr Color magenta = {255, 0, 255};
/// Predefined color light magenta
constexpr Color lightMagenta = {255, 127, 255};
/// Predefined color black
constexpr Color black = {0, 0, 0};
/// Predefined color white
constexpr Color white = {255, 255, 255};
/// Predefined color light grey
constexpr Color lightGrey = {63, 63, 63};
/// Predefined color middle grey
constexpr Color grey = {127, 127, 127};
/// Predefined color dark grey
constexpr Color darkGrey = {191, 191, 191};

}// namespace obd::gfx
