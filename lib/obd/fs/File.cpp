/**
 * @file File.cpp
 * @author argawaen
 * @date 07/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "File.h"
#include "data/DataUtils.h"
#include <utility>
#include "FileSystem.h"

namespace obd::fs {

TextFile::TextFile(std::shared_ptr<FileSystem> fileSystem) :
    _openMode{ios::none},fs{std::move(fileSystem)} {}

TextFile::~TextFile() {
    if (isOpened()) {
        close();
    }
}
TextFile::TextFile(std::shared_ptr<FileSystem> fileSystem, const Path& path, const ios& openMode) :
    _openMode{ios::none},fs{std::move(fileSystem)} {
    open(path, openMode);
}

bool TextFile::open(const Path& path, const ios& openMode) {
    if (_openMode != ios::none)
        return false;
#ifdef ARDUINO
    return false;
#else
    _openMode = openMode;
    if (_openMode == ios::in) {
        fileStream.open(fs->toStdPath(path), std::ios::in);
    } else if (_openMode == ios::app) {
        fileStream.open(fs->toStdPath(path), std::ios::out | std::ios::app);
    } else if (_openMode == ios::out) {
        fileStream.open(fs->toStdPath(path), std::ios::out);
    } else {
        _openMode = ios::none;
    }
    if (!fileStream.is_open())
        _openMode = ios::none;
    return _openMode != ios::none;
#endif
}

void TextFile::close() {
    if (_openMode == ios::none)
        return;
    _openMode = ios::none;
#ifdef ARDUINO
#else
    fileStream.close();
#endif
}
bool TextFile::isOpened() const { return _openMode != ios::none; }

char TextFile::read() {
    if (_openMode != ios::in)
        return 0;
#ifdef ARDUINO
    return 0;
#else
    char readChar=0;
    fileStream.read(&readChar, 1);
    return readChar;
#endif
}

std::string TextFile::readLine(size_t max_size, bool keepEndLines) {
    if (_openMode != ios::in)
        return {};
    std::string result;
#ifdef ARDUINO
    return std::strnig();
#else
    size_t count = 0;
    do {
        char readChar = read();
        if (!keepEndLines && (readChar == '\n' || readChar == '\r'))
            break;
        result.push_back(readChar);
        ++count;
    } while (fileStream && result.back() != '\n' && count < max_size);
#endif
    return result;
}

void TextFile::write(const char data) {
    if (_openMode != ios::out && _openMode != ios::app)
        return;
#ifdef ARDUINO
    return 0;
#else
    fileStream.put(data);
#endif
}
void TextFile::write(const std::string& data) {
    if (_openMode != ios::out && _openMode != ios::app)
        return;
#ifdef ARDUINO
    return 0;
#else
    for (auto writeChar : data)
        fileStream.put(writeChar);
#endif
}
bool TextFile::available() const {
#ifdef ARDUINO
    return false;
#else
    return !fileStream.eof();
#endif
}

}// namespace obd::fs
