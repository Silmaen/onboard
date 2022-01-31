/**
 * @file Object.h
 * @author argawaen
 * @date 17/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "config.h"
#include <typeinfo>
#include "native/OString.h"

namespace obd::core::base {
/**
 * @brief Base un-copiable object
 */
class Object {
public:
    Object()              = default;
    virtual ~Object()     = default;
    Object(const Object&) = delete;
    Object(Object&&)      = delete;
    Object& operator=(const Object&) = delete;
    Object& operator=(Object&&) = delete;

    /**
    * @brief Retrieve the driver's name based on its type
    * @return Driver's name as string
    */
    [[nodiscard]] virtual OString name() const;

    /**
     * @brief Retrieve the driver's type hash
     * @return Driver's type as hash
     */
    [[nodiscard]] virtual size_t type()const;

    /**
     * @brief Init object
     */
    virtual void init();

    /**
     * @brief Terminate object, inverse of init.
     */
    virtual void terminate();

    /**
     * @brief Frame Object
     */
    virtual void update(){}

    /**
     * @brief Check the object
     * @return True if OK
     */
    virtual bool check();

    /**
     * @brief Check for initialization
     * @return True if initialized
     */
    [[nodiscard]] bool initialized() const { return _initialized; }

    /**
     * @brief Helper to get type code
     * @tparam T The type to get the code
     * @return The code of type
     */
    template<class T>size_t code(){return typeid(T).hash_code();}

protected:
    /// Initialization state
    bool _initialized = false;
};

}// namespace obd::core::base
