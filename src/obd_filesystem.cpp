/**
 * @author Silmaen
 * @date 12/05/2021.
 */

#include "obd_filesystem.h"
#include <LittleFS.h>
#include "obd_system.h"
#include "obd_systemtime.h"

namespace obd::filesystem {


void fsDriver::init() {
    LittleFS.begin();
    auto cl = getDriverAs<time::clock>("SystemClock");
    if (cl != nullptr)
        setTimeCb(cl->getDate);
}

void fsDriver::printInfo() {
    if (getParent() == nullptr)
        return;
    println(F(" ----- FILESYSTEM INFORMATION -----"));
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
}

void fsDriver::pwd() {
    println(curPath.get());
}

void fsDriver::ls(const String& /*options*/) {
    if (getParent() == nullptr)
        return;
    auto d = LittleFS.openDir(curPath.get());
    print(F("Content of: "));
    println(curPath.get());
    while (d.next()) {
        if (d.isDirectory()) {
            print(F("d "));
        } else {
            print(F("  "));
        }
        print(static_cast<int>(d.fileSize()));
        print(F(" "));
        print(time::clock::formatTime(d.fileTime()));
        print(F(" "));
        println(d.fileName());
    }
}

void fsDriver::cd(const String& where) {
    if (getParent() == nullptr)
        return;
    if (where.isEmpty()) {
        println(F("cd: Invalid Void path"));
        return;
    }
    makePath(where);
    if (!LittleFS.exists(tempPath.get())) {
        print(F("cd: Path '"));
        print(tempPath.get());
        println(F("'does not exists"));
        return;
    }
    print(F("cd: goto "));
    println(tempPath.get());
    curPath = tempPath;
}

File fsDriver::open(const String& filename, const String& mode) {
    makePath(filename);
    return LittleFS.open(tempPath.get().c_str(), mode.c_str());
}

void fsDriver::mkdir(const String& directory) {
    if (directory.isEmpty()) {
        println(F("mkdir: Invalid Void path"));
        return;
    }
    makePath(directory);
    if (LittleFS.exists(tempPath.get())) {
        print(F("mkdir: Path '"));
        print(tempPath.get());
        println(F("'does not exists"));
        return;
    }
    LittleFS.mkdir(tempPath.get());
}

void fsDriver::rm(const String& _path) {
    if (_path.isEmpty()) {
        println(F("rm: Invalid Void path"));
        return;
    }
    makePath(_path);
    if (!LittleFS.exists(tempPath.get())) {
        print(F("rm: Path '"));
        print(tempPath.get());
        println(F("'does not exists"));
        return;
    }
    LittleFS.remove(tempPath.get());
}

bool fsDriver::treatCommand(const core::command& cmd) {
    if (cmd.isCmd(F("pwd"))) {
        pwd();
        return true;
    }
    if (cmd.isCmd(F("ls"))) {
        ls(cmd.getParams());
        return true;
    }
    if (cmd.isCmd(F("cd"))) {
        cd(cmd.getParams());
        return true;
    }
    if (cmd.isCmd(F("mkdir"))) {
        mkdir(cmd.getParams());
        return true;
    }
    if (cmd.isCmd(F("rm"))) {
        rm(cmd.getParams());
        return true;
    }
    if (cmd.isCmd(F("cat"))) {
        cat(cmd.getParams());
        return true;
    }
    return false;
}

void fsDriver::printHelp() {
    println(F("Help for Filesystem"));
    println(F("pwd           print the working directory"));
    println(F("ls            list the content of the current directory"));
    println(F("cd    <dir>   change current directory"));
    println(F("mkdir <dir>   make a new directory"));
    println(F("rm    <path>  remove a file or directory"));
    println(F("cat   <file>  display the content of a file"));
    println();
}

bool fsDriver::exists(const String& _path) {
    makePath(_path);
    return LittleFS.exists(tempPath.get());
}

void fsDriver::cat(const String& _path) {
    makePath(_path);
    if (!exists(tempPath.get())) {
        if (getParent() != nullptr) {
            print(F("ERROR: file '"));
            print(tempPath.get());
            println(F("' does not exists"));
        }
        return;
    }
    File file = LittleFS.open(tempPath.get(), "r");
    if (!file.isFile()) {
        if (getParent() != nullptr) {
            println(F("ERROR: only files can be displayed"));
        }
        return;
    }
    while (file.available() > 0) {
        print(file.readString());
    }
    file.close();
    println();
}

void fsDriver::makePath(const String& _path) {
    tempPath = path{_path};
    tempPath.makeAbsolute(curPath.get());
    tempPath.simplify();
}

void fsDriver::setTimeCb(time_t (*cb)()){
    LittleFS.setTimeCallback(cb);
}

}// namespace obd::filesystem
