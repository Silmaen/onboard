/**
 * @author Silmaen
 * @date 13/06/2021
 */

#include "obd_inputsource.h"

namespace obd::network {

String sourceToStrPrompt(const source& e){
    switch (e) {
    case network::source::NONE:
        return(F(" none   > '"));
    case network::source::USB:
        return(F(" usb    > '"));
    case network::source::UART_0:
        return(F(" uart   > '"));
    case network::source::TELNET:
        return(F(" telnet > '"));
    }
}

}//namespace obd::network
