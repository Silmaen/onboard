/**
 * @author Silmaen
 * @date 29/05/2021
 */

#include <obd_basedriver.h>
#include <obd_system.h>

namespace obd::core {

Print* baseDriver::getParentPrint() {
    if (parent == nullptr)
        return nullptr;
    return parent->getPrint();
}

}// namespace obd::core
