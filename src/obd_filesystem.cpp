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
    if (getParent() != nullptr) {
        auto cl = getParent()->getDriverAs<time::clock>("SystemClock");
        if (cl != nullptr)
            setTimeCb(cl->getDate);
    }
}

void fsDriver::printInfo() {
    if (getParent() == nullptr)
        return;
    getParentPrint()->println(F(" ----- FILESYSTEM INFORMATION -----"));
    FSInfo64 infos{};
    LittleFS.info64(infos);
    getParentPrint()->print(F("File System Size  : "));
    getParentPrint()->println(static_cast<int>(infos.totalBytes));
    getParentPrint()->print(F("File System used  : "));
    getParentPrint()->println(static_cast<int>(infos.usedBytes));
    getParentPrint()->print(F("FS block Size     : "));
    getParentPrint()->println(static_cast<int>(infos.blockSize));
    getParentPrint()->print(F("FS page Size      : "));
    getParentPrint()->println(static_cast<int>(infos.pageSize));
    getParentPrint()->print(F("Max open files:   : "));
    getParentPrint()->println(static_cast<int>(infos.maxOpenFiles));
    getParentPrint()->print(F("Max path length:  : "));
    getParentPrint()->println(static_cast<int>(infos.maxPathLength));
}

void fsDriver::pwd() {
    if (getParent() == nullptr)
        return;
    getParentPrint()->println(curPath.get());
}

void fsDriver::ls(const String& /*options*/) {
    if (getParent() == nullptr)
        return;
    auto d = LittleFS.openDir(curPath.get());
    getParentPrint()->print(F("Content of: "));
    getParentPrint()->println(curPath.get());
    while (d.next()) {
        if (d.isDirectory()) {
            getParentPrint()->print(F("d "));
        } else {
            getParentPrint()->print(F("  "));
        }
        getParentPrint()->print(static_cast<int>(d.fileSize()));
        getParentPrint()->print(F(" "));
        getParentPrint()->print(time::clock::formatTime(d.fileTime()));
        getParentPrint()->print(F(" "));
        getParentPrint()->println(d.fileName());
    }
}

void fsDriver::cd(const String& where) {
    if (getParent() == nullptr)
        return;
    if (where.isEmpty()) {
        if (getParent() != nullptr)
            getParentPrint()->println(F("cd: Invalid Void path"));
        return;
    }
    makePath(where);
    if (!LittleFS.exists(tempPath.get())) {
        if (getParent() != nullptr) {
            getParentPrint()->print(F("cd: Path '"));
            getParentPrint()->print(tempPath.get());
            getParentPrint()->println(F("'does not exists"));
        }
        return;
    }
    if (getParent() != nullptr) {
        getParentPrint()->print(F("cd: goto "));
        getParentPrint()->println(tempPath.get());
    }
    curPath = tempPath;
}

File fsDriver::open(const String& filename, const String& mode) {
    makePath(filename);
    return LittleFS.open(tempPath.get().c_str(), mode.c_str());
}

void fsDriver::mkdir(const String& directory) {
    if (directory.isEmpty()) {
        if (getParent() != nullptr)
            getParentPrint()->println(F("mkdir: Invalid Void path"));
        return;
    }
    makePath(directory);
    if (LittleFS.exists(tempPath.get())) {
        if (getParent() != nullptr) {
            getParentPrint()->print(F("mkdir: Path '"));
            getParentPrint()->print(tempPath.get());
            getParentPrint()->println(F("'does not exists"));
        }
        return;
    }
    LittleFS.mkdir(tempPath.get());
}

void fsDriver::rm(const String& _path) {
    if (_path.isEmpty()) {
        if (getParent() != nullptr)
            getParentPrint()->println(F("rm: Invalid Void path"));
        return;
    }
    makePath(_path);
    if (!LittleFS.exists(tempPath.get())) {
        if (getParent() != nullptr) {
            getParentPrint()->print(F("rm: Path '"));
            getParentPrint()->print(tempPath.get());
            getParentPrint()->println(F("'does not exists"));
        }
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
    if (getParent() == nullptr)
        return;
    getParentPrint()->println(F("Help for Filesystem"));
    getParentPrint()->println(F("pwd           print the working directory"));
    getParentPrint()->println(F("ls            list the content of the current directory"));
    getParentPrint()->println(F("cd    <dir>   change current directory"));
    getParentPrint()->println(F("mkdir <dir>   make a new directory"));
    getParentPrint()->println(F("rm    <path>  remove a file or directory"));
    getParentPrint()->println(F("cat   <file>  display the content of a file"));
    getParentPrint()->println();
}

bool fsDriver::exists(const String& _path) {
    makePath(_path);
    return LittleFS.exists(tempPath.get());
}

void fsDriver::cat(const String& _path) {
    makePath(_path);
    if (!exists(tempPath.get())) {
        if (getParent() != nullptr) {
            getParentPrint()->print(F("ERROR: file '"));
            getParentPrint()->print(tempPath.get());
            getParentPrint()->println(F("' does not exists"));
        }
        return;
    }
    File file = LittleFS.open(tempPath.get(), "r");
    if (!file.isFile()) {
        if (getParent() != nullptr) {
            getParentPrint()->println(F("ERROR: only files can be displayed"));
        }
        return;
    }
    while (file.available() > 0) {
        getParentPrint()->print(file.readString());
    }
    file.close();
    getParentPrint()->println();
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
