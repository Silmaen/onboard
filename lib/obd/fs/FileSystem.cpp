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
#include "core/System.h"
#include "time/Clock.h"

namespace obd::fs {

bool FileSystem::init() {
    if (getParent()== nullptr)
        return false;
#ifdef ARDUINO
#ifdef ESP8266
    initialized = LittleFS.begin();
#endif
#else
    basePath    = std::filesystem::current_path() / "data";
#endif
    auto tempClock = getParent()->getDriver<time::Clock>();
    if (tempClock != nullptr)
        setTimeCb(tempClock->getDate);
    return core::BaseDriver::init();;
}

void FileSystem::end() {
    core::BaseDriver::end();
#ifdef ESP8266
    LittleFS.end();
#endif
}

bool FileSystem::exists(const Path& path) const {
    if (!initialized())
        return false;
#ifdef ARDUINO
#ifdef ESP8266
    return LittleFS.exists(p.name().c_str());
#endif
#else
    return std::filesystem::exists(toStdPath(path));
#endif
}

bool FileSystem::isDir(const Path& path) const {
    if (!initialized())
        return false;
#ifdef ARDUINO
#ifdef ESP8266
    if (!LittleFS.exists(p.toString().c_str())) {
        return false;
    }
    return LittleFS.open(p.toString().c_str(), "r").isDirectory();
#endif
#else
    return std::filesystem::is_directory(toStdPath(path));
#endif
}

bool FileSystem::isFile(const Path& path) const {
    if (!initialized())
        return false;
#ifdef ARDUINO
#ifdef ESP8266
    if (!LittleFS.exists(p.toString().c_str())) {
        return false;
    }
    return LittleFS.open(p.toString().c_str(), "r").isFile();
#endif
#else
    return std::filesystem::is_regular_file(toStdPath(path));
#endif
}

bool FileSystem::mkdir(const Path& path, bool parents, bool existsOk) {
    if (!initialized()) {
        return false;
    }
    if (isFile(path)) {
        return false;
    }
    if (exists(path)) {
        return existsOk;
    }
    Path dir(path);
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
        for (auto& pp : list_parents) {
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
    if (exists(dir.parent()))
        return std::filesystem::create_directory(toStdPath(dir));
    return false;
#endif
}

bool FileSystem::rmdir(const Path& path, bool recursive, bool notExistsOk) {
    if (!initialized()) {
        return false;
    }
    if (isFile(path)) {
        return false;
    }
    if (!isDir(path)) {
        return notExistsOk;
    }
    Path dir(path);
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
    for (auto& f : ls) {
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
    if (recursive)
        return std::filesystem::remove_all(toStdPath(dir));
    if (listDir(dir).empty())
        return std::filesystem::remove(toStdPath(dir));
    return false;

#endif
}

std::vector<Path> FileSystem::listDir(const Path& path) const {
    Path work{path};
    if (path.toString().empty()) {
        work = currentWorkingDir;
    }
    if (!initialized() || !isDir(work)) {
        return {};
    }
    std::vector<Path> listFiles;
#ifdef ARDUINO
#ifdef ESP8266
    Dir dir = LittleFS.openDir(work.toString().c_str());
    while (dir.next()) {
        std::string name(dir.fileName().c_str());
        ls.emplace_back(name);
    }
#endif
#else
    auto stdPath = toStdPath(work);
    for (const auto& directoryEntry : std::filesystem::directory_iterator(stdPath)) {
        listFiles.push_back(Path(toInternalPath(directoryEntry.path())));
        listFiles.back().makeRelative(work);
    }
#endif
    return listFiles;
}

bool FileSystem::rm(const Path& path) const {
    if (!initialized()) {
        return false;
    }
    if (!isFile(path)) {
        return false;
    }
    Path dir(path);
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

bool FileSystem::cd(const Path& path) {
    if (!initialized()) {
        return false;
    }
    Path newPath = path;
    newPath.makeAbsolute(currentWorkingDir);
    if (isDir(newPath)) {
        currentWorkingDir = newPath;
        return true;
    }
    return false;
}

std::filesystem::path FileSystem::toStdPath(const Path& path) const {
    if (path.isAbsolute())
        return basePath / path.toStdPath();
    return basePath / currentWorkingDir.toStdPath() / path.toStdPath();
}

Path FileSystem::toInternalPath(const std::filesystem::path& path) const {
    return Path("/" + relative(path, basePath).generic_string());
}

bool FileSystem::touch(const Path& path){
    if (!initialized())
        return false;
    Path absolutePath = path;
    absolutePath.makeAbsolute(currentWorkingDir);
    if (isFile(absolutePath))
        return true;
    if (isDir(absolutePath)) {
        return false;
    }
    if (!isDir(absolutePath.parent())) {
        return false;
    }
    TextFile touchedFile(shared_from_this(), absolutePath, ios::out);
    touchedFile.close();
    return true;
}

bool FileSystem::treatCommand([[maybe_unused]]const core::Command& cmd) {
    return false;
}

void FileSystem::printInfo() {
    if (!initialized())
        return;
    println(F(" ----- FILESYSTEM INFORMATION -----"));
#ifdef ARDUINO
    FSInfo64 infos{};
    LittleFS.info64(infos);
    print(F("File System Size  : "));
    println(static_cast<int>(infos.totalBytes));
    print(F("File System used  : "));
    println(static_cast<int>(infos.usedBytes));
    print(F("FS block Size     : "));
    println(static_cast<int>(infos.blockSize));
    print(F("FS page Size      : "));
    println(static_cast<int>(infos.pageSize));
    print(F("Max open files:   : "));
    println(static_cast<int>(infos.maxOpenFiles));
    print(F("Max path length:  : "));
    println(static_cast<int>(infos.maxPathLength));
#endif
}

void FileSystem::printHelp() {}

void FileSystem::setTimeCb([[maybe_unused]]time_t (*callBack)()) {
#ifdef ARDUINO
    LittleFS.setTimeCallback(cb);
#endif
}

}// namespace obd::fs
