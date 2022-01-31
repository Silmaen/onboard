/**
 * @file DataUtils.cpp
 * @author argawaen
 * @date 14/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "DataUtils.h"

namespace obd::data {


/**
 * @brief
 * @param str
 * @param from
 * @param to
 * @return
 */
OString ReplaceAll(OString str, const OString& from, const OString& to) {
#ifdef ARDUINO
    str.replace(from, to);
#else
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != OString::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();// Handles case where 'to' is a substring of 'from'
    }
#endif
    return str;
}

/**
 * @brief Split string
 * @param data The string to split
 * @param delimiter The delimiter used to split
 * @return The string components
 */
std::vector<OString> split(const OString& data,
                               const OString& delimiter) {
    std::vector<OString> strings;
    OString::size_type pos  = 0;
    OString::size_type prev = 0;
    while ((pos = data.find(delimiter, prev)) != OString::npos) {
        if (pos != prev)// no void items
            strings.push_back(data.substr(prev, pos - prev));
        prev = pos + 1;
    }
    strings.push_back(data.substr(prev));
    return strings;
}

/**
 * @brief Merge a list of strings
 * @param from Begin iterator in the string list
 * @param to End iterator in the string list
 * @param delimiter The delimiter to add between the parts
 * @return The merged string
 */
OString merge(const std::vector<OString>::const_iterator& from,
                  const std::vector<OString>::const_iterator& to,
                  const OString& delimiter) {
    if (from == to)
        return "";
    auto it(from);
    OString result = *it;
    ++it;
    for (; it != to; ++it) {
        result += delimiter + *it;
    }
    return result;
}


/**
 * @brief Merge a list of strings
 * @param strings The list of string
 * @param delimiter The delimiter to add between the parts
 * @return The merged string
 */
OString merge(const std::vector<OString>& strings,
                  const OString& delimiter) {
    return merge(strings.begin(), strings.end(), delimiter);
}

}// namespace obd::data
