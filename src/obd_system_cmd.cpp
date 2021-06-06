//
// Created by damien.lachouette on 11/05/2021.
//

#include <obd_system_cmd.h>

namespace obd::core {

bool command::isCmd(const String& cmp) const {
    return cmdline.substring(0, cmdline.indexOf(' ')) == cmp;
}

void command::printCmd(Print& st) const {
    switch (from) {
    case source::NONE:
        st.print(" none   > ");
        break;
    case source::USB:
        st.print(" usb    > ");
        break;
    case source::UART_0:
        st.print(" uart   > ");
        break;
    case source::TELNET:
        st.print(" telnet > ");
        break;
    }
    st.println(cmdline.c_str());
}

String command::getParams() const {
    return cmdline.substring(cmdline.indexOf(' '));
}

}// namespace obd::core
