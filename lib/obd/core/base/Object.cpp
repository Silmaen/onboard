/**
 * @file Object.cpp
 * @author argawaen
 * @date 17/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Object.h"
#include <algorithm>


namespace obd::core::base {
/// Definition of digits
static const OString digits{"0123456789"};

/**
 * @brief Get the last digits of a string
 * @param str The string
 * @return The last digits
 */
OString::size_type lastDigit(const OString& str) {
    OString::size_type idx = -1;
    for (OString::size_type i = 0; i < str.length(); ++i) {
        if (std::any_of(digits.begin(), digits.end(), [str, i](char charTest) { return str[i] == charTest; })) {
            idx = i;
        }
    }
    return idx;
}

OString Object::name() const {
    OString result{typeid(*this).name()};
    if (result.back() == 'E')
        result.pop_back();
    return result.substr(lastDigit(result) + 1, result.length());
}

void Object::init() {
    _initialized = true;
}

bool Object::check() {
    return initialized();
}

void Object::terminate() {
    _initialized = false;
}

size_t Object::type() const {
    return typeid(*this).hash_code();
}

}// namespace obd::core::base
