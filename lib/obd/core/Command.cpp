/**
 * @file Command.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Command.h"

namespace obd::core {


bool Command::isCmd(const std::string& cmp) const {
    return cmdline.substr(0, cmdline.find(' ')) == cmp;;
}

void Command::printCmd(com::Output& output) const {
    output.print(com::sourceToStrPrompt(from));
    output.print(cmdline);
    output.println(F("'"));
}

std::string Command::getParams() const {
    if (cmdline.find(' ') == std::string::npos)
        return {};
    return cmdline.substr(cmdline.find(' ') + 1);
}

}// namespace obd::core
