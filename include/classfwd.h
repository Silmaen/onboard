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
class system;
class MultiPrint;
class command;
class StatusLed;
}// namespace core

/**
 * @brief namespace for the file system
 */
namespace filesystem {
class fsDriver;
}// namespace filesystem

/**
 * @brief namespace for the network system
 */
namespace network {
class netDriver;
}// namespace network

/**
 * @brief Namespace for time and clock
 */
namespace time {
class clock;
}// namespace time

/**
 * @brief Namespace for managing the web server
 */
namespace webserver {
class webDriver;
}// namespace webserver

}// namespace obd
