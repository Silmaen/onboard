//
// Created by damien.lachouette on 11/05/2021.
//

#include <obd_system_cmd.h>

namespace obd {
namespace system {

bool system::command::isCmd(const char *cmp) {
    size_t l1 = strlen(cmp);
    //Serial.print("--- '");Serial.print(cmp);Serial.print("' '");Serial.print(cmdline);Serial.print("' ");Serial.print((int)l1);Serial.print(" ");Serial.println(size());
    if (l1 > size()) {
        return false;
    }
    for(size_t i=0;i<l1;++i){
        if (cmdline[i] != cmp[i])
            return false;
    }
    return (l1 == size()) || ((l1 < size()) && (cmdline[l1] == ' '));
}

void command::printCmd(Stream &st) {
    switch(from){
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

} // namespace system
} // namespace obd
