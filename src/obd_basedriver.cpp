/**
 * \author argawaen 
 * \date 29/05/2021
 *
 */

#include <obd_basedriver.h>
#include <obd_system.h>

namespace obd {
namespace core {

Print* baseDriver::getParentPrint() {
    if (parent == nullptr)
        return nullptr;
    return parent->getPrint();
}

}// namespace core
}// namespace obd
