/**
 * @file Source.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "native/fakeArduino.h"
#include <string>

namespace obd::com {
/**
 * @brief Source's types.
 */
enum class Source {
    NONE,  ///< no source
    USB,   ///< USB source
    UART_0,///< Serial source
    TELNET ///< Telnet source
};

/**
 * @brief Convert a source enum to a string valuer
 * @param e The input source enum
 * @return The corresponding string
 */
inline std::string sourceToStrPrompt(const Source& e) {
    switch (e) {
    case Source::NONE:
        return (F(" none   > '"));
    case Source::USB:
        return (F(" usb    > '"));
    case Source::UART_0:
        return (F(" uart   > '"));
    case Source::TELNET:
        return (F(" telnet > '"));
    }
    return (F(""));
}

}// namespace obd::com
