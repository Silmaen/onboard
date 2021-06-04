//
// Created by damien.lachouette on 12/05/2021.
//

#pragma once
#include "obd_basedriver.h"
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
    explicit driver(core::system *p = nullptr) : baseDriver(p) { strcpy(curPath, "/"); };
    ~driver()              = default;
    driver(const driver &) = default;
    driver(driver &&)      = default;
    driver &operator=(const driver &) = default;
    driver &operator=(driver &&) = default;
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
    bool treatCommand(const core::command &cmd) override;

    /**
     * @brief display command help
     */
    void printHelp() override;

    /**
     * @brief get the name of the driver
     * @return driver name
     */
    [[nodiscard]] std::string getName() const override { return "Filesystem"; }

    /**
     * @brief open a file
     * @param filename name of the file
     * @param mode opening mode
     * @return the file handler
     */
    static File open(char *filename, char *mode);


    /**
     * @brief print working directory
     * @param output the console where to write
     */
    void pwd();

    /**
     * @brief list files in the current directory
     * @param options options to the ls command
     */
    void ls(const char *options);

    /**
     * @brief change directory
     * @param where the new path
     */
    void cd(const char *where);

    /**
     * @brief create a new directory
     * @param directory the directory to create
     */
    void mkdir(const char *directory);

    /**
     * @brief remove a file or a directory (with its content)
     * @param path what to remove
     */
    void rm(const char *path);

private:
    /**
     * @brief the current file path
     */
    char curPath[maxPathLength]{};

    /**
     * @brief buffer path to make intermediate computations
     */
    char tempPath[maxPathLength]{};

    /**
     * @brief function that convert a path to absolute if not already stored it to tempPath
     * @param path the path to convert
     */
    void makeAbsolute(const char *path);

    /**
     * @brief compact the path containing ".." or "."
     * @param path the path to compact
     */
    void compactPath(char *path);
};

}// namespace obd::filesystem
