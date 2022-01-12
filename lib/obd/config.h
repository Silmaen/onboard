/**
 * @file config.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <cstdint>

/**
 * @brief Configuration parameters
 */
namespace obd::config {

/// Base length of a command line
constexpr uint8_t commandBufferLength = 30;

/// Default led state period
constexpr uint64_t ledPeriod = 2000000;

/// Name of the file to store time
constexpr char tsSave[]{"/dev/time"};

/// interval between 2 save of the timestamp
constexpr uint64_t saveInterval = 60000000;

}// namespace obd::config
