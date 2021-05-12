//
// Created by damien.lachouette on 12/05/2021.
//

#pragma once
#include <Arduino.h>
#include "config.h"
#include <FS.h>

namespace obd {
namespace internal {

class filesystem_impl {
public:
    filesystem_impl(){curPath="/";};
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
    void printInfo(Stream *output);

    /**
     * @brief print working directory
     * @param output the console where to write
     */
    void pwd(Stream* output);

    /**
     * @brief list files in the current directory
     * @param output the console where to write
     * @param options options to the ls command
     */
    void ls(Stream* output, char* options);

    /**
     * @brief change directory
     * @param output the console where to write
     * @param where
     */
    void cd(Stream* output, char* where);
private:
    char curPath[maxPathLength]{};
};
} // namespace internal

extern internal::filesystem_impl filesystem;

} // namespace obd
