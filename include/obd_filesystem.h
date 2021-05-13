//
// Created by damien.lachouette on 12/05/2021.
//

#pragma once
#include <Print.h>
#include "config.h"
#include <FS.h>

namespace obd {
namespace internal {

class filesystem_impl {
public:

    filesystem_impl(){ strcpy(curPath, "/");};
    ~filesystem_impl() = default;
    filesystem_impl(const filesystem_impl &) = default;
    filesystem_impl(filesystem_impl &&) = default;
    filesystem_impl &operator=(const filesystem_impl &) = default;
    filesystem_impl &operator=(filesystem_impl &&) = default;

    /**
     * @brief initialize file system
     */
    static void init();

    /**
     * @brief open a file
     * @param filename name of the file
     * @param mode opening mode
     * @return the file handler
     */
    static File open(char* filename, char* mode);

    /**
     * @brief print the file system information in the given stream
     * @param output the stream onto write
     */
    static void printInfo(Print &output);

    /**
     * @brief print working directory
     * @param output the console where to write
     */
    void pwd(Print& output);

    /**
     * @brief list files in the current directory
     * @param output the console where to write
     * @param options options to the ls command
     */
    void ls(Print& output, const char* options);

    /**
     * @brief change directory
     * @param output the console where to write
     * @param where the new path
     */
    void cd(Print& output, const char* where);

    /**
     * @brief create a new directory
     * @param output the console where to write
     * @param directory the directory to create
     */
    void mkdir(Print& output, const char* directory);

    /**
     * @brief remove a file or a directory (with its content)
     * @param output the console where to write
     * @param path what to remove
     */
    void rm(Print& output, const char* path);
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
     * @brief function that convert a path to absolute if not already stor it to tempPath
     * @param path the path to convert
     *
     */
    void makeAbsolute(Print &output, const char* path);
};
} // namespace internal

extern internal::filesystem_impl filesystem;

} // namespace obd
