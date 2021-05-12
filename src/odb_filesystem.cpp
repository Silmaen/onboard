//
// Created by damien.lachouette on 12/05/2021.
//

#include "obd_filesystem.h"
#include <LittleFS.h>

namespace obd {
namespace internal {


void filesystem_impl::init() {
    LittleFS.begin();
}

void filesystem_impl::printInfo(Stream *output) {
FSInfo64 infos{};
LittleFS.info64(infos);
output->print(F("File System Size: "));
output->println(static_cast<int>(infos.totalBytes));
output->print(F("File System used: "));
output->println(static_cast<int>(infos.usedBytes));
output->print(F("FS block Size:    "));
output->println(static_cast<int>(infos.blockSize));
output->print(F("FS page Size:     "));
output->println(static_cast<int>(infos.pageSize));
output->print(F("Max open files:   "));
output->println(static_cast<int>(infos.maxOpenFiles));
output->print(F("Max path length:  "));
output->println(static_cast<int>(infos.maxPathLength));
}

void filesystem_impl::pwd(Stream* output){

}

void filesystem_impl::ls(Stream *output, char *options) {
}

void filesystem_impl::cd(Stream *output, char *where) {
}


File filesystem_impl::open(char *filename, char *mode) {
    return LittleFS.open(filename, mode);
}


} // namespace internal
} // namespace obd
