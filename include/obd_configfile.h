/**
 * @author Silmaen
 * @date 07/06/2021
 */
#pragma once

#include "Basedriver.h"
#include "obd_path.h"
#include <map>
#include <utility>

namespace obd::file {

/**
 * @brief class to handle the config files
 */
class ConfigFile {
public:
    /**
     * @brief Constructor with parent filesystem
     * @param p the parent file system
     */
    explicit ConfigFile(std::shared_ptr<FileSystem>  p = nullptr) :
        fs{std::move(p)} {}

    /**
     * @brief Constructor with parent
     * @param p the parent system
     */
    explicit ConfigFile(core::System* p = nullptr);

    /**
     * @brief check if the config file for the given driver exists
     * @param driverName the name of the driver
     * @return
     */
    [[nodiscard]] bool configExists(const String& driverName) const;

    /**
     * @brief load a config from file (only if file exists)
     * @param driverName the name of the driver to load
     */
    void loadConfig(const String& driverName);

    /**
     * @brief save into file the given data
     * @param driverName the name of the driver to save
     */
    void saveConfig(const String& driverName) const;

    /**
     * @brief add a parameter to the list
     * @param key the key name
     * @param val the value to store as a String
     */
    void addConfigParameter(const String& key, const String& val);

    /**
     * @brief check existence of a key in the current data
     * @param key the key name
     * @return true if data exists under the given key
     */
    [[nodiscard]] bool hasKey(const String& key) const;

    /**
     * @brief return the value under the given key (or void string)
     * @param key the key name
     * @return the value under the given key
     */
    [[nodiscard]] const String& getKey(const String& key) const;

    /**
     * @brief reset database
     */
    void clear();

private:
    /// link to the filesystem
    std::shared_ptr<FileSystem> fs = nullptr;

    /// list of the file items
    std::map<String, String> fileContent;
};

}// namespace obd::file
