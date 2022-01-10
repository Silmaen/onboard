/**
 * @author Silmaen
 * @date 07/07/2021
 */

#pragma once
#include "Basedriver.h"
#include <vector>

namespace obd::core {

/**
 * @brief simple class to handle the list of drivers
 */
class DriverManager {
public:
    /**
     * @brief add a driver into the list
     *
     * the driver type must be unique
     * @tparam T the type of the driver to add
     * @param parent the parent system that should be attached to the driver
     */
    template<class T>
    void addDriver(System* parent) {
        if (!std::is_base_of<BaseDriver, T>::value)// only class derived from BaseDriver is allowed
            return;
        if (getDriver<T>() != nullptr) // ensure driver type is unique in the list
            return;
        drivers.push_back(std::make_shared<T>(parent));
    }

    /**
     * @brief get the driver of the given type
     * @tparam T th type of the driver to search
     * @return a pointer to the driver, return nullptr for any error
     */
    template<class T>
    std::shared_ptr<T> getDriver() {
        auto search = find_if(begin(), end(), [](const std::shared_ptr<BaseDriver>& b) { return typeid(T) == typeid(*b); });
        if (search == end())
            return nullptr;
        return std::static_pointer_cast<T>(*search);
    }

    /**
     * @brief get begin iterator of the driver list
     * @return begin iterator of the driver list
     */
    std::vector<std::shared_ptr<BaseDriver>>::iterator begin(){return drivers.begin();}

    /**
     * @brief get end iterator of the driver list
     * @return end iterator of the driver list
     */
    std::vector<std::shared_ptr<BaseDriver>>::iterator end(){return drivers.end();}
private:

    /// list of the drivers
    std::vector<std::shared_ptr<BaseDriver>> drivers{};
};

}// namespace obd::core
