/**
 * @author Silmaen
 * @date 11/05/2021.
 */

#include <obd_system_cmd.h>

namespace obd::core {

bool command::isCmd(const String& cmp) const {
    return cmdline.substring(0, cmdline.indexOf(' ')) == cmp;
}

void command::printCmd(Print& st) const {
    switch (from) {
    case source::NONE:
        st.print(F(" none   > '"));
        break;
    case source::USB:
        st.print(F(" usb    > '"));
        break;
    case source::UART_0:
        st.print(F(" uart   > '"));
        break;
    case source::TELNET:
        st.print(F(" telnet > '"));
        break;
    }
    st.print(cmdline.c_str());
    st.println(F("'"));
}

String command::getParams() const {
    return cmdline.substring(cmdline.indexOf(' ') + 1);
}

}// namespace obd::core
