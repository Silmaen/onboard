//
// Created by damien.lachouette on 12/05/2021.
//

#include "obd_filesystem.h"
#include <LittleFS.h>

namespace obd {

namespace filesystem {

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
    getParentPrint()->println(curPath);
}

void driver::ls(const char *options) {
    if (getParent() == nullptr)
        return;
    auto d = LittleFS.openDir(curPath);
    getParentPrint()->print("Content of: ");
    getParentPrint()->println(curPath);
    while (d.next()) {
        if (d.isDirectory()) {
            getParentPrint()->print("d ");
        } else {
            getParentPrint()->print("  ");
        }
        getParentPrint()->print(static_cast<int>(d.fileSize()));
        getParentPrint()->print(" ");
        getParentPrint()->print(d.fileTime());
        getParentPrint()->print(" ");
        getParentPrint()->println(d.fileName());
    }
}

void driver::cd(const char *where) {
    if (getParent() == nullptr)
        return;
    if (where == nullptr) {
        if(getParent() != nullptr)
            getParentPrint()->println(F("cd: Invalid Void path"));
        return;
    }
    makeAbsolute(where);
    if (!LittleFS.exists(tempPath)) {
        if(getParent() != nullptr)
            getParentPrint()->println(F("cd: Path does not exists"));
        return;
    }
    if(getParent() != nullptr) {
        getParentPrint()->print(F("cd: goto "));
        getParentPrint()->println(tempPath);
    }
    strcpy(curPath, tempPath);
}

File driver::open(char *filename, char *mode) {
    return LittleFS.open(filename, mode);
}

void driver::mkdir(const char *directory) {
    if (directory == nullptr) {
        if(getParent() != nullptr)
            getParentPrint()->println(F("mkdir: Invalid Void path"));
        return;
    }
    makeAbsolute(directory);
    if (LittleFS.exists(tempPath)) {
        if(getParent() != nullptr)
            getParentPrint()->println(F("mkdir: Path already exists"));
        return;
    }
    LittleFS.mkdir(tempPath);
}

void driver::rm(const char *path) {
    if (path == nullptr) {
        if(getParent() != nullptr)
            getParentPrint()->println(F("rm: Invalid Void path"));
        return;
    }
    makeAbsolute(path);
    if (!LittleFS.exists(tempPath)) {
        if(getParent() != nullptr)
            getParentPrint()->println(F("rm: Path does not exists"));
        return;
    }
    LittleFS.remove(tempPath);
}

void driver::makeAbsolute(const char *path) {
    if (path == nullptr) {
        strcpy(curPath,"/");
        return;
    }
    if (path[0] == '/') {// already absolute
        strcpy(tempPath, path);
        compactPath(tempPath);
        return;
    }
    if (strlen(path) + strlen(curPath) + 1 >= maxPathLength) {
        if(getParent() != nullptr)
            getParentPrint()->println(F("Path too long"));
        compactPath(curPath);
        return;
    }
    strcpy(tempPath, curPath);
    if (tempPath[strlen(tempPath) - 1] != '/')
        strcat(tempPath, "/");
    strcat(tempPath, path);
    compactPath(tempPath);
}

void driver::compactPath(char *path) {
    if (path == nullptr)// avoid errors!!
        return;
    size_t len = strlen(path);
    if ((len <= 1) || (strcmp(path, "/..") == 0) || (strcmp(path, "/.") == 0)) {// 0 length string: return root!
        path[0] = '/';
        path[1] = '\0';
        return;
    }


    // "/" at the end (except for root), with recursion to remove all the '/' at the end
    if ((len > 1) && (path[len - 1] == '/')) {
        path[len - 1] = '\0';
        // path is modified, redo the compact
        compactPath(path);
    }

    // "/./" or "/../" in the beginning or middle
    for (size_t j = 0; j < len - 2; ++j) {
        if ((path[j] == '/') && (path[j + 1] == '.') && (path[j + 2] == '/')) {
            len -= 2;
            for (size_t i = j; i < len; ++i) {
                path[i] = path[i + 2];
            }
            path[len] = '\0';
            // path is modified, redo the compact
            compactPath(path);
        }
    }
    for (size_t j = 0; j < len - 3; ++j) {
        if ((path[j] == '/') && (path[j + 1] == '.') && (path[j + 2] == '.') && (path[j + 3] == '/')) {
            len -= 3;
            for (size_t i = j; i < len; ++i) {
                path[i] = path[i + 3];
            }
            path[len] = '\0';
            // path is modified, redo the compact
            compactPath(path);
        }
    }

    // remove "/." at the end
    if ((path[len - 2] == '/') && (path[len - 1] == '.')) {
        path[len - 2] = '\0';
        // path is modified, redo the compact
        compactPath(path);
    }

    // remove "/.." at the end (case with only "/.." should already be treated by the begining
    if ((path[len - 3] == '/') && (path[len - 2] == '.') && (path[len - 1] == '.')) {
        // get previous '/' char
        for (long int i = len - 4; i >= 0; --i) {
            if (path[i] == '/') {
                path[i] = '\0';
                compactPath(path);
                break;
            }
        }
        return;
    }
}

bool driver::treatCommand(const core::command &cmd) {
    if (cmd.isCmd("pwd")) {
        pwd();
        return true;
    }
    if (cmd.isCmd("ls")) {
        ls(cmd.getParams());
        return true;
    }
    if (cmd.isCmd("cd")) {
        cd(cmd.getParams());
        return true;
    }
    if (cmd.isCmd("mkdir")) {
        mkdir(cmd.getParams());
        return true;
    }
    if (cmd.isCmd("rm")) {
        rm(cmd.getParams());
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
    getParentPrint()->println();
}

}// namespace filesystem
}// namespace obd
