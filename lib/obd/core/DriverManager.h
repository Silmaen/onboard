/**
 * @file DriverManager.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "BaseDriver.h"
#include <memory>
#include <vector>

namespace obd::core {

class System;

/**
 * @brief Simple class to handle the list of drivers
 */
class DriverManager {
public:
    /**
     * @brief Add a driver into the list
     *
     * The driver type must be unique
     * @tparam T The driver's type to add
     * @param parent The parent system that should be attached to the driver
     */
    template<class T>
    void addDriver(System* parent) {
        if (!std::is_base_of<BaseDriver, T>::value)// only class derived from BaseDriver is allowed
            return;
        if (getDriver<T>() != nullptr)// ensure driver type is unique in the list
            return;
        drivers.push_back(std::make_shared<T>(parent));
    }

    /**
     * @brief Get the driver of the given type
     * @tparam T The driver's type to search
     * @return A pointer to the driver, return nullptr for any error
     */
    template<class T>
    std::shared_ptr<T> getDriver() {
        auto search = find_if(begin(), end(), [](const std::shared_ptr<BaseDriver>& b) { return typeid(T) == typeid(*b); });
        if (search == end())
            return nullptr;
        return std::static_pointer_cast<T>(*search);
    }

    /**
     * @brief Get begin iterator of the driver list
     * @return Begin iterator of the driver list
     */
    std::vector<std::shared_ptr<BaseDriver>>::iterator begin() { return drivers.begin(); }

    /**
     * @brief Get end iterator of the driver list
     * @return End iterator of the driver list
     */
    std::vector<std::shared_ptr<BaseDriver>>::iterator end() { return drivers.end(); }

private:
    /// List of the drivers
    std::vector<std::shared_ptr<BaseDriver>> drivers{};
};

}// namespace obd::core
