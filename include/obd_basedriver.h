/**
 * @author Silmaen
 * @date 29/05/2021
 */
#pragma once

#include "classfwd.h"
#include "obd_system_cmd.h"
#include <Print.h>
#include <memory>

namespace obd::core {

/**
 * @brief base implementation of a system driver
 */
class baseDriver {
public:
    /**
     * @brief constructor with parent
     * @param p the parent system
     */
    explicit baseDriver(system* p = nullptr) :
        parent{p} {}

    /**
     * @brief attach a new parent to this driver
     * @param p the parent
     */
    virtual void attachParent(system* p) {
        parent = p;
    }

    /**
     * @brief get the parent system
     * @return the parent
     */
    [[nodiscard]] system* getParent() const {
        return parent;
    }

    /**
     * @brief get the outputs from the parent system
     * @return the output
     */
    Print* getParentPrint();

    /**
     * @brief retrieve the name of the driver
     * @return name of the driver
     */
    [[nodiscard]] virtual String getName() const = 0;

    /**
     * @brief initialize the driver
     */
    virtual void init() = 0;

    /**
     * @brief print the driver infos
     */
    virtual void printInfo() = 0;

    /**
     * @brief procedure called each frame
     * @param delta the delta from last update
     */
    virtual void update(int64_t delta) = 0;

    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    virtual bool treatCommand([[maybe_unused]] const command& cmd) {
        return false;
    }

    /**
     * @brief display driver help on commands
     */
    virtual void printHelp() = 0;

    /**
     * @brief load and apply parameters in the config file
     */
    virtual void loadConfigFile() = 0;

    /**
     * @brief save the driver parameter in file
     */
    virtual void saveConfigFile() const = 0;

    /**
     * @brief try to print in MultiStream
     * @param in input to display
     * @return number of byte writen
     */
    size_t print(const __FlashStringHelper* in);

    /**
     * @brief try to print in MultiStream
     * @param in input to display
     * @return number of byte writen
     */
    size_t print(const String& in);

    /**
     * @brief try to print in MultiStream
     * @param in input to display
     * @return number of byte writen
     */
    size_t print(const char* in);

    /**
     * @brief try to print in MultiStream
     * @param in input to display
     * @return number of byte writen
     */
    size_t print(char in);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(unsigned char in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(int16_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(uint16_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(int32_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(uint32_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(int64_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(uint64_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t print(double in, int format = 2);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t print(const Printable& in);


    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t println(const __FlashStringHelper* in);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t println(const String& in);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t println(const char* in);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t println(char in);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(unsigned char in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(int16_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(uint16_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(int32_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(uint32_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(int64_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(uint64_t in, int format = DEC);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @param format format of the display
     * @return number of byte writen
     */
    size_t println(double in, int format = 2);

    /**
     * @brief try to print in MultiStream with a newline
     * @param in input to display
     * @return number of byte writen
     */
    size_t println(const Printable& in);

    /**
     * @brief try to print in MultiStream with a newline
     * @return number of byte writen
     */
    size_t println();

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
    std::shared_ptr<baseDriver> getDriver(const String& name);

    /**
     * @brief get the driver by its name and convert it to desired type
     * @tparam T the desired output type (must inherit from baseDriver)
     * @param name the driver name
     * @return the driver (nullptr if not exists or if template class does not inherit from baseDriver)
     */
    template<class T>
    std::shared_ptr<T> getDriverAs(const String& name){
        if (!std::is_base_of<baseDriver, T>::value)
            return nullptr;
        std::shared_ptr<baseDriver> a = getDriver(name);
        if (a == nullptr)
            return nullptr;
        return std::static_pointer_cast<T>(a);
    }

private:
    /// pointer to the parent system
    system* parent = nullptr;
};

}// namespace obd::core
