/**
 * @file ConfigFile.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <map>
#include "native/OString.h"
#include <memory>

namespace obd::fs {

class FileSystem;

/**
 * @brief Class to handle the config files
 */
class ConfigFile {
public:
    /**
     * @brief Constructor with parent filesystem
     * @param fileSystem The parent file system
     */
    explicit ConfigFile(std::shared_ptr<FileSystem>  fileSystem = nullptr) :
        fs{std::move(fileSystem)} {}

    /**
     * @brief Check if the config file for the given driver exists
     * @param driverName The name of the driver
     * @return
     */
    [[nodiscard]] bool configExists(const OString& driverName) const;

    /**
     * @brief Load a config from file (only if file exists)
     * @param driverName The name of the driver to load
     */
    void loadConfig(const OString& driverName);

    /**
     * @brief Save into file the given data
     * @param driverName The name of the driver to save
     */
    void saveConfig(const OString& driverName) const;

    /**
     * @brief Add a parameter to the list
     * @param key The key name
     * @param val The value to store as a String
     */
    void addConfigParameter(const OString& key, const OString& val);

    /**
     * @brief Check existence of a key in the current data
     * @param key The key name
     * @return True if data exists under the given key
     */
    [[nodiscard]] bool hasKey(const OString& key) const;

    /**
     * @brief Return the value under the given key (or void string)
     * @param key The key name
     * @return The value under the given key
     */
    [[nodiscard]] const OString& getKey(const OString& key) const;

    /**
     * @brief Reset database
     */
    void clear();

private:
    /// Link to the filesystem
    std::shared_ptr<FileSystem> fs = nullptr;

    /// List of the file items
    std::map<OString, OString> fileContent;
};

}// namespace obd::fs
