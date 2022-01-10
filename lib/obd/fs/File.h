/**
 * @file File.h
 * @author argawaen
 * @date 06/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Path.h"
#ifdef ARDUINO
#ifdef ESP8266
#include <FS.h>
#include <LittleFS.h>
#endif
#else
#include <fstream>
#endif

namespace obd::fs {

class FileSystem;

/**
 * @brief Opening mode of files
 */
enum struct ios { none,
                  in,
                  out,
                  app };

/**
 * @brief Class handling file
 */
class TextFile {
public:
    /**
     * @brief Default Constructor
     * @param fileSystem link to the filesystem
     */
    explicit TextFile(FileSystem* fileSystem);

    /**
     * @brief Destructor
     */
    ~TextFile();
    /**
     * @brief Constructor that open the file
     * @param fileSystem link to the filesystem
     * @param path The path of the file
     * @param openMode The open mode of the file.
     */
    TextFile(FileSystem* fileSystem, const Path& path, const ios& openMode = ios::in);

    /**
     * @brief Open the file
     * @param path The path to the file
     * @param openMode The open mode of the file
     * @return True if open succeed
     */
    bool open(const Path& path, const ios& openMode);

    /**
     * @brief Close and release the file
     */
    void close();

    /**
     * @brief Check whether the file is opened
     * @return
     */
    [[nodiscard]] bool isOpened() const;

    /**
     * @brief Read one char of the file
     * @return The read char
     */
    [[nodiscard]] char read();

    /**
     * @brief Is something is available for reading
     * @return True if not at EOF
     */
    bool available()const;

    /**
     * @brief Read a whole line to the file
     * @param max_size The maximum number of char to read.
     * @return The line read
     */
    [[nodiscard]] std::string readLine(size_t max_size = 255);

    /**
     * @brief Write a data to the file
     * @param data The data to write
     */
    void write(char data);

    /**
     * @brief Write a data to the file
     * @param data The data to write
     */
    void write(const std::string& data);

private:
    /// The OpenMode of the file
    ios _openmode;

    /// Pointer to the file System
    FileSystem* fs;
#ifdef ARDUINO
#ifdef ESP8266
#endif
#else
    std::fstream fileStream;
#endif
};

}// namespace obd::fs
