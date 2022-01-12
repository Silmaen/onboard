/**
 * @file BaseDriver.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include <algorithm>
#include "BaseDriver.h"


/// Definition of digits
static const std::string digits{"0123456789"};

/**
 * @brief Get the last digits of a string
 * @param str The string
 * @return The last digits
 */
std::string::size_type lastDigit(const std::string& str) {
    std::string::size_type idx = -1;
    for (std::string::size_type i = 0; i < str.length(); ++i) {
        if (std::any_of(digits.begin(), digits.end(), [str, i](char charTest) { return str[i] == charTest; })) {
            idx = i;
        }
    }
    return idx;
}

std::string obd::core::BaseDriver::getName() const {
    std::string result{typeid(*this).name()};
    if (result.back()== 'E')
        result.pop_back();
    return result.substr(lastDigit(result) + 1, result.length());
}
