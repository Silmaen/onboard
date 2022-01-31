/**
 * @file DataUtils.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "native/OString.h"
#include <vector>

namespace obd::data {

/// Just an empty string
static const OString emptyString;

/**
 * @brief
 * @param str
 * @param from
 * @param to
 * @return
 */
OString ReplaceAll(OString str, const OString& from, const OString& to);

/**
 * @brief Split string
 * @param data The string to split
 * @param delimiter The delimiter used to split
 * @return The string components
 */
std::vector<OString> split(const OString& data,
                               const OString& delimiter);

/**
 * @brief Merge a list of strings
 * @param from Begin iterator in the string list
 * @param to End iterator in the string list
 * @param delimiter The delimiter to add between the parts
 * @return The merged string
 */
OString merge(const std::vector<OString>::const_iterator& from,
                  const std::vector<OString>::const_iterator& to,
                  const OString& delimiter);


/**
 * @brief Merge a list of strings
 * @param strings The list of string
 * @param delimiter The delimiter to add between the parts
 * @return The merged string
 */
OString merge(const std::vector<OString>& strings,
                  const OString& delimiter);

}// namespace obd::data
