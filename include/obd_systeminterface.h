/**
 * \author Silmaen
 * \date 06/07/2021
 */

#pragma once
#include "classfwd.h"
#include "obd_system_cmd.h"
#include "obd_drivertypes.h"
#include <Print.h>
#include <memory>

namespace obd::core {

/**
 * @brief base class for interfacing with the system
 */
class SystemInterface {
public:
    /**
     * @brief constructor with parent
     * @param p the parent system
     */
    explicit SystemInterface(System* p = nullptr) :
    parent{p} {}

    /**
     * @brief get the parent system
     * @return the parent
     */
    [[nodiscard]] System* getParent() const {
        return parent;
    }

    /**
     * @brief get the outputs from the parent system
     * @return the output
     */
    [[nodiscard]] Print* getParentPrint() const;

    /**
 * @brief try to print in MultiStream
 * @param in input to display
 * @return number of byte writen
 */
    size_t print(const __FlashStringHelper* in) const;

    /**
     * @brief try to print in MultiStream
     * @param in input to display
     * @return number of byte writen
     */
    size_t print(const String& in) const;

    /**
     * @brief try to print in MultiStream
     * @param in input to display
     * @return number of byte writen
     */
    size_t print(const char* in) const;

    /**
     * @brief try to print in MultiStream
     * @param in input to display
     * @return number of byte writen
     */
    size_t print(char in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(unsigned char in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(int16_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(uint16_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(int32_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(uint32_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(int64_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(uint64_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(double in, int format = 2) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t print(const Printable& in) const;


    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t println(const __FlashStringHelper* in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t println(const String& in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t println(const char* in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t println(char in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(unsigned char in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(int16_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(uint16_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(int32_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(uint32_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(int64_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(uint64_t in, int format = DEC) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(double in, int format = 2) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t println(const Printable& in) const;

    /**
     * @brief try to print in MultiStream with a newline
     * @return number of byte writen
     */
    size_t println() const;

    /**
     * @brief try to add a command int eh parent
     * @param cmd command to add
     */
    void pushCommand(const command& cmd) const;

    /**
     * @brief get a driver by its name
     * @param name the name of the driver
     * @return the driver (nullptr if not exists)
     */
    [[nodiscard]] std::shared_ptr<BaseDriver> getDriver(const String& name) const;

    /**
     * @brief get a driver by its name
     * @param type the class name of the driver
     * @return the driver (nullptr if not exists)
     */
    [[nodiscard]] std::shared_ptr<BaseDriver> getDriver(const DriverType& type) const;

    /**
     * @brief get the driver by its name and convert it to desired type
     * @tparam T the desired output type (must inherit from baseDriver)
     * @param name the driver name
     * @return the driver (nullptr if not exists or if template class does not inherit from baseDriver)
     */
    template<class T>
    [[nodiscard]] std::shared_ptr<T> getDriverAs(const String& name)const{
        if (!std::is_base_of<BaseDriver, T>::value)
            return nullptr;
        std::shared_ptr<BaseDriver> a = getDriver(name);
        if (a == nullptr)
            return nullptr;
        return std::static_pointer_cast<T>(a);
    }

    /**
     * @brief get the driver by its name and convert it to desired type
     * @tparam T the desired output type (must inherit from baseDriver)
     * @return the driver (nullptr if not exists or if template class does not inherit from baseDriver)
     */
    template<class T>
    [[nodiscard]] std::shared_ptr<T> getDriverAs() const {
        if (!std::is_base_of<BaseDriver, T>::value)
            return nullptr;
        T tmp{(System*) this};
        std::shared_ptr<BaseDriver> a = getDriver(tmp.getType());
        if (a == nullptr)
            return nullptr;
        return std::static_pointer_cast<T>(a);
    }
private:
    /// pointer to the parent system
    System* parent = nullptr;
};

}// namespace obd::core

