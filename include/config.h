/**
 * @author Silmaen
 * @date 11/05/2021.
 */
#pragma once

#include <WString.h>
#include <cstdint>

namespace obd::config {

/// the name of the author
constexpr char author[]{"Damien Lachouette"};

/// current version of the program
constexpr char version[]{"1.0"};

/// base length of a command line
constexpr uint8_t commandBufferLength = 30;

/// default led state period
constexpr uint64_t ledPeriod = 2000000;

/// default name for the device
constexpr char defaultHostname[]{"Onboard12a85a"};

/// interval between 2 save of the timestamp
constexpr uint64_t saveInterval = 60000000;

/// name of the file to store time
constexpr char tsSave[]{"/dev/time"};

/// base path for webserver
constexpr char baseWeb[]{"/www"};

}// namespace obd::config
