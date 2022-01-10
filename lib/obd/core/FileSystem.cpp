/**
 * @file FileSystem.cpp
 * @author argawaen
 * @date 07/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "FileSystem.h"
#ifdef ESP8266
#include <FS.h>
#include <LittleFS.h>
#endif
#include "com/Communicator.h"

/**
 * @brief Global file system item
 */
obd::core::FileSystem fs;

namespace obd::core {

FileSystem::FileSystem() : currentWorkingDir(Path("/")) {}

bool FileSystem::begin() {
#ifdef ARDUINO
#ifdef ESP8266
  initialized = LittleFS.begin();
#endif
#else
  initialized = true;
  basePath = std::filesystem::current_path() / "data";
#endif
  return initialized;
}

void FileSystem::end() {
  initialized = false;
#ifdef ESP8266
  LittleFS.end();
#endif
}

bool FileSystem::exists(const Path &p) const {
  if (!initialized)
    return false;
#ifdef ARDUINO
#ifdef ESP8266
  return LittleFS.exists(p.name().c_str());
#endif
#else
  return std::filesystem::exists(toStdPath(p));
#endif
}

bool FileSystem::isDir(const Path &p) const {
  if (!initialized)
    return false;
#ifdef ARDUINO
#ifdef ESP8266
  if (!LittleFS.exists(p.toString().c_str())) {
    return false;
  }
  return LittleFS.open(p.toString().c_str(), "r").isDirectory();
#endif
#else
  return std::filesystem::is_directory(toStdPath(p));
#endif
}

bool FileSystem::isFile(const Path &p) const {
  if (!initialized)
    return false;
#ifdef ARDUINO
#ifdef ESP8266
  if (!LittleFS.exists(p.toString().c_str())) {
    return false;
  }
  return LittleFS.open(p.toString().c_str(), "r").isFile();
#endif
#else
  return std::filesystem::is_regular_file(toStdPath(p));
#endif
}

bool FileSystem::mkdir(const Path &p, bool parents, bool existsOk) {
  if (!initialized) {
    Port.newline(com::Verbosity::Error);
    Port.print("fs.mkdir: file system not initialized");
    return false;
  }
  if (isFile(p)) {
    Port.newline(com::Verbosity::Error);
    Port.print("fs.mkdir: Already exists as a file");
    return false;
  }
  if (exists(p)) {
    if (!existsOk) {
      Port.newline(com::Verbosity::Error);
      Port.print("fs.mkdir: Already exists");
    }
    return existsOk;
  }
  Path dir(p);
  if (!dir.isAbsolute()) {
    dir.makeAbsolute(currentWorkingDir);
  }
#ifdef ARDUINO
  if (!parents) {
    if (!isDir(dir.parent())) {
      return false;
    } else {
#ifdef ESP8266
      return LittleFS.mkdir(dir.toString().c_str());
#endif
    }
  } else {
    std::vector<Path> list_parents;
    Path parent = dir.parent();
    while (!parent.isRoot()) {
      list_parents.push_back(parent);
      parent = parent.parent();
    }
    for (auto &pp : list_parents) {
      if (!isDir(pp)) {
        if (exists(pp))
          return false;
#ifdef ESP8266
        if (!LittleFS.mkdir(pp.toString().c_str()))
          return false;
#endif
      }
    }
    return true;
  }
#else
  if (parents)
    return std::filesystem::create_directories(toStdPath(dir));
  else {
    if (exists(dir.parent()))
      return std::filesystem::create_directory(toStdPath(dir));
    else {
      Port.newline(com::Verbosity::Error);
      Port.print("fs.mkdir: the parent directory '" + dir.parent().toString() +
                 "' not exist");
      return false;
    }
  }
#endif
}

bool FileSystem::rmdir(const Path &p, bool recurcive, bool notExistsOk) {
  if (!initialized) {
    Port.newline(com::Verbosity::Error);
    Port.print("fs.rmdir: file system not initialized");
    return false;
  }
  if (isFile(p)) {
    Port.newline(com::Verbosity::Error);
    Port.print("fs.rmdir: need a folder not a file");
    return false;
  }
  if (!isDir(p)) {
    if (!notExistsOk) {
      Port.newline(com::Verbosity::Error);
      Port.print("fs.rmdir: the directory does not exist");
    }
    return notExistsOk;
  }
  Path dir(p);
  if (!dir.isAbsolute()) {
    dir.makeAbsolute(currentWorkingDir);
  }
#ifdef ARDUINO
  auto ls = listDir(p);
  if (!recurcive) {
    if (ls.empty()) {
#ifdef ESP8266
      LittleFS.rmdir(p.toString().c_str());
#endif
      return true;
    }
    return false;
  }
  for (auto &f : ls) {
    if (isFile(f)) {
      if (!rm(f))
        return false;
    }
    if (isDir(f)) {
      if (rmdir(f, true, true))
        return false;
    }
  }
  return true;
#else
  if (recurcive)
    return std::filesystem::remove_all(toStdPath(dir));
  else {
    if (listDir(dir).empty())
      return std::filesystem::remove(toStdPath(dir));
    else {
      Port.newline(com::Verbosity::Error);
      Port.print("fs.rmdir: the directory is not empty");
      return false;
    }
  }
#endif
}

std::vector<Path> FileSystem::listDir(const Path &p) const {
  Path work{p};
  if (p.toString().empty()) {
    work = currentWorkingDir;
  }
  if (!initialized || !isDir(work)) {
    Port.newline(com::Verbosity::Error);
    Port.print("fs.listDir: file system not initialized");
    return {};
  }
  std::vector<Path> ls;
#ifdef ARDUINO
#ifdef ESP8266
  Dir dir = LittleFS.openDir(work.toString().c_str());
  while (dir.next()) {
    std::string name(dir.fileName().c_str());
    ls.emplace_back(name);
  }
#endif
#else
  auto path = toStdPath(work);
  for (const auto &f : std::filesystem::directory_iterator(path)) {
    ls.push_back(Path(toInternalPath(f.path())));
    ls.back().makeRelative(work);
  }
#endif
  return ls;
}

bool FileSystem::rm(const Path &p) const {
  if (!initialized) {
    Port.newline(com::Verbosity::Error);
    Port.print("fs.rm: file system not initialized");
    return false;
  }
  if (!isFile(p)) {
    Port.newline(com::Verbosity::Error);
    Port.print("fs.rm: the files does not exist or is a directory");
    return false;
  }
  Path dir(p);
  if (!dir.isAbsolute()) {
    dir.makeAbsolute(currentWorkingDir);
  }
#ifdef ARDUINO
#ifdef ESP8266
  return LittleFS.remove(p.toString().c_str());
#endif
#else
  return std::filesystem::remove(toStdPath(dir));
#endif
}

bool FileSystem::cd(const Path &p) {
  if (!initialized) {
    Port.newline(com::Verbosity::Error);
    Port.print("fs.cd: file system not initialized");
    return false;
  }
  Path dd = p;
  dd.makeAbsolute(currentWorkingDir);
  if (isDir(dd)) {
    currentWorkingDir = dd;
    return true;
  }
  Port.newline(com::Verbosity::Error);
  Port.print("fs.cd: Directory does not exist");
  return false;
}

std::filesystem::path FileSystem::toStdPath(const Path &path) const {
  if (path.isAbsolute()) {
    return basePath / path.toStdPath();
  } else {
    return basePath / currentWorkingDir.toStdPath() / path.toStdPath();
  }
}

Path FileSystem::toInternalPath(const std::filesystem::path &path) const {
  return Path("/" + relative(path, basePath).generic_string());
}

bool FileSystem::touch(const Path &p) const {
  Path dd = p;
  dd.makeAbsolute(currentWorkingDir);
  if (isFile(dd))
    return true;
  if (isDir(dd)) {
    Port.newline(com::Verbosity::Error);
    Port.print("fs.touch: already a dir");
    return false;
  }
  if (!isDir(dd.parent())) {
    Port.newline(com::Verbosity::Error);
    Port.print("fs.touch: parent '" + dd.parent().toString() + "' not a dir");
    return false;
  }
  TextFile f(dd, ios::out);
  f.close();
  return true;
}

} // namespace cpt::core
