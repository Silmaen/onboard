//
// Created by damien.lachouette on 11/05/2021.
//

#include <obd_system_cmd.h>

namespace obd {
namespace core {

bool command::isCmd(const char *cmp) const{
    size_t l1 = strlen(cmp);
    //Serial.print("--- '");Serial.print(cmp);Serial.print("' '");Serial.print(cmdline);Serial.print("' ");Serial.print((int)l1);Serial.print(" ");Serial.println(size());
    if (l1 > size()) {
        return false;
    }
    for (size_t i = 0; i < l1; ++i) {
        if (cmdline[i] != cmp[i])
            return false;
    }
    return (l1 == size()) || ((l1 < size()) && (cmdline[l1] == ' '));
}

void command::printCmd(Print &st) const{
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
    st.println(cmdline);
}

const char *command::getParams() const {
    for (size_t i = 1; i < size(); ++i) {
        if ((cmdline[i - 1] == ' ') && (cmdline[i] != ' '))
            return &(cmdline[i]);
    }
    return nullptr;
}

}// namespace core
}// namespace obd
