/**
 * @author Silmaen
 * @date 11/05/2021.
 */

#pragma once
#include <WString.h>
#include <cstdint>

namespace obd {

/// the name of the author
constexpr char author[]{"Damien Lachouette"};

/// current version of the program
constexpr char version[]{"1.0"};

/// base length of a command line
constexpr uint8_t commandBufferLength = 30;

/// maximum length of a file system path
constexpr uint8_t maxPathLength = 30;

/// default led state period
constexpr uint16_t ledPeriod = 2000;

/// default name for the device
constexpr char defaultHostname[]{"Onboard12a85a"};

}// namespace obd
