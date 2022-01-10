/**
 * @file File.cpp
 * @author argawaen
 * @date 07/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "File.h"
#include "FileSystem.h"

namespace obd::core {

TextFile::TextFile() : _openmode{ios::none} {};

TextFile::~TextFile() {
  if (isOpened())
    close();
}
TextFile::TextFile(const Path &p, const ios &openMode) : _openmode{ios::none} {
  open(p, openMode);
}

bool TextFile::open(const Path &p, const ios &openMode) {
  if (_openmode != ios::none)
    return false;
#ifdef ARDUINO
  return false;
#else
  _openmode = openMode;
  if (_openmode == ios::in)
    fileStream.open(fs.toStdPath(p), std::ios::in);
  else if (_openmode == ios::app)
    fileStream.open(fs.toStdPath(p), std::ios::out | std::ios::app);
  else if (_openmode == ios::out)
    fileStream.open(fs.toStdPath(p), std::ios::out);
  else {
    _openmode = ios::none;
  }
  if (!fileStream.is_open())
    _openmode = ios::none;
  return _openmode != ios::none;
#endif
}

void TextFile::close() {
  if (_openmode == ios::none)
    return;
  _openmode = ios::none;
#ifdef ARDUINO
#else
  fileStream.close();
#endif
}
bool TextFile::isOpened() const { return _openmode != ios::none; }

char TextFile::read() {
  if (_openmode != ios::in)
    return 0;
#ifdef ARDUINO
  return 0;
#else
  char c;
  fileStream.read(&c, 1);
  return c;
#endif
}

std::string TextFile::readLine(size_t max_size) {
  if (_openmode != ios::in)
    return {};
#ifdef ARDUINO
  return std::strnig();
#else
  std::string result;
  size_t count = 0;
  do {
    result.push_back(read());
    ++count;
  } while (fileStream && result.back() != '\n' && count < max_size);
  return result;
#endif
}

void TextFile::write(const char data) {
  if (_openmode != ios::out && _openmode != ios::app)
    return;
#ifdef ARDUINO
  return 0;
#else
  fileStream.put(data);
#endif
}
void TextFile::write(const std::string &data) {
  if (_openmode != ios::out && _openmode != ios::app)
    return;
#ifdef ARDUINO
  return 0;
#else
  for (auto c : data)
    fileStream.put(c);
#endif
}

} // namespace cpt::core
