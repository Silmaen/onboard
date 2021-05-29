//
// Created by damien.lachouette on 11/05/2021.
//

#pragma once
#include <cstdint>
namespace obd {

/// base length of a command line
constexpr uint8_t commandBufferLength = 30;

/// maximum length of a file system path
constexpr uint8_t maxPathLength = 30;

/// default led state period
constexpr uint16_t ledPeriod = 2000;

constexpr uint16_t maxConfigFileSize = 1024;

constexpr char defaultHostname[]{"Onboard_12a85a"};

} // namespace obd
