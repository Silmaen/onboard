/**
 * @author Silmaen
 * @date 13/06/2021
 */

#pragma once
#include <WString.h>

namespace obd::network {
/**
 * @brief type of the sources
 */
enum class source {
    NONE,  ///< no source
    USB,   ///< USB source
    UART_0,///< Serial source
    TELNET ///< Telnet source
};

String sourceToStrPrompt(const source& e);

}//namespace obd::network
