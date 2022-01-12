/**
 * @author Silmaen
 * @date 29/05/2021
 */

#include <obd_basedriver.h>
#include <obd_system.h>

namespace obd::core {

/// Definition of digits
static const String digits{"0123456789"};

/**
 * @brief Get the last digits of a string
 * @param str the string
 * @return the las digits
 */
int lastDigit(const String& str) {
    int idx = -1;
    for (uint16_t i = 0; i < str.length(); ++i) {
        if (std::any_of(digits.begin(), digits.end(), [str, i](char c) { return str[i] == c; })) {
            idx = i;
        }
    }
    return idx;
}

String BaseDriver::getName() const {
    String resu{typeid(*this).name()};
    if (resu.isEmpty())
        return "";
    if (resu[0] == 'N') {
        resu = resu.substring(1, resu.length() - 1);
    }
    return resu.substring(lastDigit(resu) + 1, resu.length());
}

}// namespace obd::core
