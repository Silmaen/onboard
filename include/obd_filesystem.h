//
// Created by damien.lachouette on 12/05/2021.
//

#pragma once
#include "obd_basedriver.h"
#include "obd_path.h"
#ifdef ARDUINO
#include <FS.h>
#include <Print.h>
#endif

/**
 * @namespace obd::filesystem
 * @brief namespace for the file system
 */
namespace obd::filesystem {

/**
 * @brief driver for the filesystem
 */
class driver : public core::baseDriver {
public:
    /**
     * @brief constructor with parent
     * @param p the parent system
     */
    explicit driver(core::system* p = nullptr) :
        baseDriver(p), curPath{F("/")}{};

    /**
     * @brief initialize file system
     */
    void init() override;

    /**
     * @brief print the file system information in the given stream
     * @param output the stream onto write
     */
    void printInfo() override;

    /**
     * @brief listen to network for commands
     */
    void update(uint64_t timestamp) override {}

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
    [[nodiscard]] String getName() const override { return F("Filesystem"); }

    /**
     * @brief open a file
     * @param filename name of the file
     * @param mode opening mode
     * @return the file handler
     */
    static File open(const String& filename, const String& mode);


    /**
     * @brief print working directory
     * @param output the console where to write
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

private:
    /**
     * @brief the current file path
     */
    path curPath{"/"};

    /**
     * @brief buffer path to make intermediate computations
     */
    path tempPath{"/"};
};

}// namespace obd::filesystem
