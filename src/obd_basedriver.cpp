/**
 * @author Silmaen
 * @date 29/05/2021
 */

#include <obd_basedriver.h>
#include <obd_system.h>
#include <cxxabi.h>

namespace obd::core {

String BaseDriver::getName() const {
    int i = 0;
    String resu(abi::__cxa_demangle(typeid(*this).name(),nullptr,nullptr, &i));
    if (i != 0)
        return "TypeError";
    return resu;
}

}// namespace obd::core
