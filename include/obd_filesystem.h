/**
 * @author Silmaen
 * @date 12/05/2021
 */

#pragma once
#include "obd_basedriver.h"
#include "obd_path.h"
#include <FS.h>
#include <Print.h>

namespace obd::filesystem {

/**
 * @brief driver for the filesystem
 */
class fsDriver : public core::baseDriver {
public:
    /**
     * @brief constructor with parent
     * @param p the parent system
     */
    explicit fsDriver(core::system* p = nullptr) :
        baseDriver(p), curPath{F("/")} {};

    /**
     * @brief initialize file system
     */
    void init() override;

    /**
     * @brief print the file system information in the given stream
     */
    void printInfo() override;

    /**
     * @brief listen to network for commands
     * @param delta the time delta from last update
     */
    void update([[maybe_unused]]int64_t delta) override {}

    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    bool treatCommand(const core::command& cmd) override;

    /**
     * @brief display command help
     */
    void printHelp() override;

    /**
     * @brief get the name of the driver
     * @return driver name
     */
    [[nodiscard]] String getName() const override { return F("FileSystem"); }

    /**
     * @brief load and apply parameters in the config file
     */
    void loadConfigFile() override {}

    /**
     * @brief save the driver parameter in file
     */
    void saveConfigFile() const override {}

    /**
     * @brief open a file
     * @param filename name of the file
     * @param mode opening mode
     * @return the file handler
     */
    File open(const String& filename, const String& mode);

    /**
     * @brief print working directory
     */
    void pwd();

    /**
     * @brief list files in the current directory
     * @param options options to the ls command
     */
    void ls(const String& options);

    /**
     * @brief change directory
     * @param where the new path
     */
    void cd(const String& where);

    /**
     * @brief create a new directory
     * @param directory the directory to create
     */
    void mkdir(const String& directory);

    /**
     * @brief remove a file or a directory (with its content)
     * @param path what to remove
     */
    void rm(const String& path);

    /**
     * @brief check the existence of a file of directory
     * @param path the path to check
     * @return true if the path exists
     */
    [[nodiscard]] bool exists(const String& path);

    /**
     * @brief display the content of a file
     * @param path the file to display
     */
    void cat(const String& path);

    /**
     * @brief set time callback
     * @param cb the time call back
     */
    static void setTimeCb(time_t (*cb)());

private:
    /**
     * @brief the current file path
     */
    path curPath{"/"};

    /**
     * @brief buffer path to make intermediate computations
     */
    path tempPath{"/"};

    /**
     * @brief create and verify a path
     * @param _path
     */
    void makePath(const String& _path);
};

}// namespace obd::filesystem
