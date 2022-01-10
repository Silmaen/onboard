/**
 * @file DataUtils.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <string>

namespace obd::data {

/// just an empty string
static const std::string emptyString;


/**
 * @brief
 * @param str
 * @param from
 * @param to
 * @return
 */
inline std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();// Handles case where 'to' is a substring of 'from'
    }
    return str;
}
}// namespace obd::data
