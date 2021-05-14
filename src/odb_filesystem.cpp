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

void filesystem_impl::pwd(Print &output) {
    output.println(curPath);
}

void filesystem_impl::ls(Print &output, const char *options) {
    auto d = LittleFS.openDir(curPath);
    output.print("Content of: ");
    output.println(curPath);
    while (d.next()) {
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
    if (where == nullptr) {
        output.println(F("cd: Invalid Void path"));
        return;
    }
    makeAbsolute(output, where);
    if (!LittleFS.exists(tempPath)) {
        output.println(F("cd: Path does not exists"));
        return;
    }
    output.print(F("cd: goto "));
    output.println(tempPath);
    strcpy(curPath, tempPath);
}

File filesystem_impl::open(char *filename, char *mode) {
    return LittleFS.open(filename, mode);
}

void filesystem_impl::mkdir(Print &output, const char *directory) {
    if (directory == nullptr) {
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
    if (path == nullptr) {
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
        strcpy(curPath,"/");
        return;
    }
    if (path[0] == '/') {// already absolute
        strcpy(tempPath, path);
        compactPath(tempPath);
        return;
    }
    if (strlen(path) + strlen(curPath) + 1 >= maxPathLength) {
        output.println(F("Path too long"));
        compactPath(curPath);
        return;
    }
    strcpy(tempPath, curPath);
    if (tempPath[strlen(tempPath) - 1] != '/')
        strcat(tempPath, "/");
    strcat(tempPath, path);
    compactPath(tempPath);
}

void filesystem_impl::compactPath(char *path) {
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

}// namespace internal
}// namespace obd
