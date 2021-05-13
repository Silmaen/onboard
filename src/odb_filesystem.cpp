//
// Created by damien.lachouette on 12/05/2021.
//

#include "obd_filesystem.h"
#include <LittleFS.h>

namespace obd {

internal::filesystem_impl filesystem;

namespace internal {

void filesystem_impl::init() {
    LittleFS.begin();
}

void filesystem_impl::printInfo(Print &output) {
    FSInfo64 infos{};
    LittleFS.info64(infos);
    output.print(F("File System Size: "));
    output.println(static_cast<int>(infos.totalBytes));
    output.print(F("File System used: "));
    output.println(static_cast<int>(infos.usedBytes));
    output.print(F("FS block Size:    "));
    output.println(static_cast<int>(infos.blockSize));
    output.print(F("FS page Size:     "));
    output.println(static_cast<int>(infos.pageSize));
    output.print(F("Max open files:   "));
    output.println(static_cast<int>(infos.maxOpenFiles));
    output.print(F("Max path length:  "));
    output.println(static_cast<int>(infos.maxPathLength));
}

void filesystem_impl::pwd(Print& output){
    output.println(curPath);
}

void filesystem_impl::ls(Print& output, const char *options) {
    auto d = LittleFS.openDir(curPath);
    output.print("Content of: ");
    output.println(curPath);
    while (d.next()){
        if (d.isDirectory()) {
            output.print("d ");
        } else {
            output.print("  ");
        }
        output.print(static_cast<int>(d.fileSize()));
        output.print(" ");
        output.print(d.fileTime());
        output.print(" ");
        output.println(d.fileName());
    }
}

void filesystem_impl::cd(Print &output, const char *where) {
    if (where == nullptr){
        output.println(F("cd: Invalid Void path"));
        return;
    }
    makeAbsolute(output, where);
    if (!LittleFS.exists(tempPath)){
        output.println(F("cd: Path does not exists"));
        return;
    }

    strcpy(curPath, tempPath);
}

File filesystem_impl::open(char *filename, char *mode) {
    return LittleFS.open(filename, mode);
}

void filesystem_impl::mkdir(Print &output, const char *directory) {
    if (directory == nullptr){
        output.println(F("mkdir: Invalid Void path"));
        return;
    }
    makeAbsolute(output, directory);
    if (LittleFS.exists(tempPath)) {
        output.println(F("mkdir: Path already exists"));
        return;
    }
    LittleFS.mkdir(tempPath);
}

void filesystem_impl::rm(Print &output, const char *path) {
    if (path == nullptr){
        output.println(F("rm: Invalid Void path"));
        return;
    }
    makeAbsolute(output, path);
    if (!LittleFS.exists(tempPath)) {
        output.println(F("rm: Path does not exists"));
        return;
    }
    LittleFS.remove(tempPath);
}

void filesystem_impl::makeAbsolute(Print &output, const char *path) {
    if (path == nullptr) {
        strcpy(tempPath, curPath);
        return;
    }
    if (path[0] == '/'){ // already absolute
        strcpy(tempPath, path);
        return;
    }
    // Todo: manage 'parent' directory: ".."
    if (strlen(path)+strlen(curPath) + 1 >= maxPathLength){
        output.println(F("Path too long"));
        return;
    }
    strcpy(tempPath, curPath);
    if (tempPath[strlen(tempPath)-1] != '/')
        strcat(tempPath, "/");
    strcat(tempPath, path);
}

} // namespace internal
} // namespace obd
