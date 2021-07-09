/**
 * \author Silmaen
 * \date 06/07/2021
 */

#pragma once

namespace obd::core {

/**
 * @brief type of the drivers
 */
enum struct DriverType {
    Unknown = 0,///< unknown type (default)
    StatusLed,  ///< driver for the status led
    UsbSerial,  ///< driver for the usb (or serial TX/RX) connexion
    FileSystem, ///< driver for the filesystem
    WifiDriver, ///< driver for the wifi & telnet server
    Clock,      ///< Driver for the clock
    WebServer,  ///< Driver for the webserver
};

}// namespace obd::core
