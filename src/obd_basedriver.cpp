/**
 * @author Silmaen
 * @date 29/05/2021
 */

#include <obd_basedriver.h>
#include <obd_system.h>

namespace obd::core {

String BaseDriver::getName() const {
    //String resu{abi::__cxa_demangle(typeid(*this).name(),nullptr,nullptr, &i)};
    String resu{typeid(*this).name()};
    size_t i = 0;
    while (i < resu.length()) {
        if (resu[i] < '0' || resu[i] > '9')
            break;
        ++i;
    }
    if (i == resu.length())
        return F("TypeError");
    return resu.substring(i);
}

}// namespace obd::core
