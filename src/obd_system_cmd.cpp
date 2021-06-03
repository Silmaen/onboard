//
// Created by damien.lachouette on 11/05/2021.
//

#include <obd_system_cmd.h>

namespace obd::core {

bool command::isCmd(const std::string& cmp) const {
    return cmdline.substr(0, cmdline.find(' ')) == cmp;
}

void command::printCmd(Print &st) const {
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

std::string command::getParams() const {
    return cmdline.substr(cmdline.find(' '));
}

}// namespace obd::core
