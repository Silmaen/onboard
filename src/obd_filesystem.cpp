/**
 * @author Silmaen
 * @date 12/05/2021.
 */

#include "obd_filesystem.h"
#include <LittleFS.h>

namespace obd::filesystem {

void driver::init() {
    LittleFS.begin();
}

void driver::printInfo() {
    if (getParent() == nullptr)
        return;
    FSInfo64 infos{};
    LittleFS.info64(infos);
    getParentPrint()->print(F("File System Size: "));
    getParentPrint()->println(static_cast<int>(infos.totalBytes));
    getParentPrint()->print(F("File System used: "));
    getParentPrint()->println(static_cast<int>(infos.usedBytes));
    getParentPrint()->print(F("FS block Size:    "));
    getParentPrint()->println(static_cast<int>(infos.blockSize));
    getParentPrint()->print(F("FS page Size:     "));
    getParentPrint()->println(static_cast<int>(infos.pageSize));
    getParentPrint()->print(F("Max open files:   "));
    getParentPrint()->println(static_cast<int>(infos.maxOpenFiles));
    getParentPrint()->print(F("Max path length:  "));
    getParentPrint()->println(static_cast<int>(infos.maxPathLength));
}

void driver::pwd() {
    if (getParent() == nullptr)
        return;
    getParentPrint()->println(curPath.get());
}

void driver::ls(const String& /*options*/) {
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
        getParentPrint()->print(d.fileTime());
        getParentPrint()->print(F(" "));
        getParentPrint()->println(d.fileName());
    }
}

void driver::cd(const String& where) {
    if (getParent() == nullptr)
        return;
    if (where.isEmpty()) {
        if (getParent() != nullptr)
            getParentPrint()->println(F("cd: Invalid Void path"));
        return;
    }
    tempPath = path{where};
    tempPath.simplify();
    if (!LittleFS.exists(tempPath.get())) {
        if (getParent() != nullptr)
            getParentPrint()->println(F("cd: Path does not exists"));
        return;
    }
    if (getParent() != nullptr) {
        getParentPrint()->print(F("cd: goto "));
        getParentPrint()->println(tempPath.get());
    }
    curPath = tempPath;
}

File driver::open(const String& filename, const String& mode) {
    tempPath = path(filename);
    tempPath.makeAbsolute(curPath.get());
    return LittleFS.open(tempPath.get().c_str(), mode.c_str());
}

void driver::mkdir(const String& directory) {
    if (directory.isEmpty()) {
        if (getParent() != nullptr)
            getParentPrint()->println(F("mkdir: Invalid Void path"));
        return;
    }
    tempPath = path{directory};
    tempPath.simplify();
    if (LittleFS.exists(tempPath.get())) {
        if (getParent() != nullptr)
            getParentPrint()->println(F("mkdir: Path already exists"));
        return;
    }
    LittleFS.mkdir(tempPath.get());
}

void driver::rm(const String& _path) {
    if (_path.isEmpty()) {
        if (getParent() != nullptr)
            getParentPrint()->println(F("rm: Invalid Void path"));
        return;
    }
    tempPath = path{_path};
    tempPath.simplify();
    if (!LittleFS.exists(tempPath.get())) {
        if (getParent() != nullptr)
            getParentPrint()->println(F("rm: Path does not exists"));
        return;
    }
    LittleFS.remove(tempPath.get());
}

bool driver::treatCommand(const core::command& cmd) {
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

void driver::printHelp() {
    if (getParent() == nullptr)
        return;
    getParentPrint()->println(F("Help for Filesystem"));
    getParentPrint()->println(F("pwd     print the working directory"));
    getParentPrint()->println(F("ls      list the content of the current directory"));
    getParentPrint()->println(F("cd      change current directory"));
    getParentPrint()->println(F("mkdir   make a new directory"));
    getParentPrint()->println(F("rm      remove a file or directory"));
    getParentPrint()->println(F("cat     display the content of a file"));
    getParentPrint()->println();
}

bool driver::exists(const String& _path) {
    tempPath = path{_path};
    tempPath.simplify();
    return LittleFS.exists(tempPath.get());
}

void driver::cat(const String& _path) {
    if (!exists(_path)) {
        if (getParent() != nullptr){
            getParentPrint()->println(F("ERROR: file does not exists"));
        }
        return;
    }
    File file = LittleFS.open(tempPath.get(), "r");
    if (!file.isFile()) {
        if (getParent() != nullptr){
            getParentPrint()->println(F("ERROR: only files can be displayed"));
        }
        return;
    }
    while(file.available()>0){
        getParentPrint()->print(file.readString());
    }
    file.close();
}

}// namespace obd::filesystem
