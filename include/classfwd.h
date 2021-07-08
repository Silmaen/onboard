/**
 * @author Silmaen
 * @date 29/05/2021
 */
#pragma once

#include "config.h"

/**
 * @brief base namespace of the project
 */
namespace obd {

/**
 * @brief configuration
 */
namespace config {
}// namespace config

/**
 * @brief namespace for the base system
 */
namespace core {
class DriverManager;
class System;
class SystemIterface;
class BaseDriver;
class MultiPrint;
class Command;
class StatusLed;
}// namespace core

/**
 * @brief namespace for the file system
 */
namespace file {
class Path;
class FileSystem;
}// namespace file

/**
 * @brief namespace for the network system
 */
namespace network {
class UsbSerial;
class WifiDriver;
class WebServer;
}// namespace network

/**
 * @brief Namespace for time and clock
 */
namespace time {
class Clock;
}// namespace time

/**
 * @brief Namespace for managing the video
 */
namespace video {
class RunCam;
}// namespace video

}// namespace obd
