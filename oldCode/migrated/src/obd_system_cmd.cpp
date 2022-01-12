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
    st.print(network::sourceToStrPrompt(from));
    st.print(cmdline.c_str());
    st.println(F("'"));
}

String command::getParams() const {
    if (cmdline.indexOf(' ') < 0)
        return String();
    return cmdline.substring(cmdline.indexOf(' ') + 1);
}

}// namespace obd::core
