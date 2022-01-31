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
    explicit TextFile(std::shared_ptr<FileSystem> fileSystem);

    /**
     * @brief Destructor
     */
    ~TextFile();
    /**
     * @brief Constructor that open the file
     * @param fileSystem Link to the filesystem
     * @param path The path of the file
     * @param openMode The open mode of the file.
     */
    TextFile(std::shared_ptr<FileSystem> fileSystem, const Path& path, const ios& openMode = ios::in);

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
     * @brief Something available for reading?
     * @return True if not at EOF
     */
    bool available()const;

    /**
     * @brief Read a whole line to the file
     * @param max_size The maximum amount of char to read.
     * @param keepEndLines If keep the end line chars.
     * @return The line read
     */
    [[nodiscard]] OString readLine(size_t max_size = 255, bool keepEndLines =true);

    /**
     * @brief Write a data to the file
     * @param data The data to write
     */
    void write(char data);

    /**
     * @brief Write a data to the file
     * @param data The data to write
     */
    void write(const OString& data);

private:
    /// The OpenMode of the file
    ios _openMode;

    /// Pointer to the file System
    std::shared_ptr<FileSystem> fs;
#ifdef ARDUINO
#ifdef ESP8266
#endif
#else
    std::fstream fileStream;
#endif
};

}// namespace obd::fs
