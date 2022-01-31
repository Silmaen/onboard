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

class System_old;

/**
 * @brief Simple class to handle the list of drivers
 */
class DriverManager {
public:
    /// Driver pointer type
    using driver = std::shared_ptr<BaseDriver>;
    /// Driver pointer type
    using reference = driver&;
    /// Iterator type
    using iterator = std::vector<driver>::iterator;
    /// Constant iterator type
    using const_iterator = std::vector<driver>::const_iterator;

    /**
     * @brief Add a driver into the list
     *
     * The driver type must be unique
     * @tparam T The driver's type to add
     * @param parent The parent system that should be attached to the driver
     * @return False if the driver is not added.
     */
    template<class T>
    bool addDriver(System_old* parent) {
        if (!std::is_base_of<BaseDriver, T>::value)// only class derived from BaseDriver is allowed
            return false;
        if (getDriver<T>() != nullptr)// ensure driver type is unique in the list
            return false;
        drivers.push_back(std::make_shared<T>(parent));
        return true;
    }

    /**
     * @brief Get the driver of the given type
     * @tparam T The driver's type to search
     * @return A pointer to the driver, return nullptr for any error
     */
    template<class T>
    std::shared_ptr<T> getDriver() {
        auto search = find_if(begin(), end(), [](const driver& driver) { return typeid(T) == typeid(*driver); });
        if (search == end())
            return nullptr;
        return std::static_pointer_cast<T>(*search);
    }

    /**
     * @brief Remove a driver from the list
     * @tparam T The Driver Type
     * @return True if the driver has been deleted
     */
    template<class T>
    bool deleteDriver() {
        auto search = find_if(begin(), end(), [](const driver&  driver) { return typeid(T) == typeid(*driver); });
        if (search == end())  // no such driver
            return false;
        drivers.erase(search);
        return true;
    }

    /**
     * @brief Get begin iterator of the driver list
     * @return Begin iterator of the driver list
     */
    iterator begin() { return drivers.begin(); }

    /**
     * @brief Get end iterator of the driver list
     * @return End iterator of the driver list
     */
    iterator end() { return drivers.end(); }

    /**
     * @brief Get begin iterator of the driver list
     * @return Begin iterator of the driver list
     */
    [[nodiscard]] const_iterator begin()const { return drivers.begin(); }

    /**
     * @brief Get end iterator of the driver list
     * @return End iterator of the driver list
     */
    [[nodiscard]] const_iterator end()const { return drivers.end(); }
    /**
     * @brief Get begin iterator of the driver list
     * @return Begin iterator of the driver list
     */
    [[nodiscard]] const_iterator cbegin()const { return drivers.cbegin(); }

    /**
     * @brief Get end iterator of the driver list
     * @return End iterator of the driver list
     */
    [[nodiscard]] const_iterator cend()const { return drivers.cend(); }

    /**
     * @brief Get a reference to the last driver
     * @return The last driver of the list
     */
    reference back(){
        return drivers.back();
    }
private:
    /// List of the drivers
    std::vector<driver> drivers{};
};

}// namespace obd::core
