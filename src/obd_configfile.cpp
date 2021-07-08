/**
 * @author Silmaen
 * @date 07/06/2021
 */

#include "obd_configfile.h"
#include <obd_filesystem.h>
#include <obd_system.h>

namespace obd::file {

ConfigFile::ConfigFile(core::System* p) {
    if (p == nullptr)
        return;
    fs = p->getDriver<file::FileSystem>();
}

bool ConfigFile::configExists(const String& driverName) const {
    if (fs == nullptr)
        return false;
    Path pp{driverName + F(".cfg")};
    pp.makeAbsolute(F("/config"));
    return fs->exists(pp.get());
}

void ConfigFile::loadConfig(const String& driverName) {
    if (!configExists(driverName))
        return;
    Path pp{driverName + F(".cfg")};
    pp.makeAbsolute(F("/config"));
    File file = fs->open(pp.get(),F("r"));
    while(file.available() > 0){
        String line = file.readStringUntil('\n');
        line.replace("\r", ""); // just in case of damned Windows format
        if (line.isEmpty()) // empty line
            continue;
        if (line.startsWith("#")) // comment line
            continue;
        int idx = line.indexOf("#");
        if (idx > 0 ) {// remove end line comments
            line = line.substring(0, idx);
        }
        line.replace(" ", ""); // remove all spaces
        idx = line.indexOf("=");
        if (idx < 0 ) // invalid line
            continue;
        addConfigParameter(line.substring(0, idx), line.substring(idx + 1));
    }
    file.close();
}

void ConfigFile::saveConfig(const String& driverName) const {
    if (fs == nullptr)
        return;
    if (fileContent.empty())
        return;
    Path pp{driverName + F(".cfg")};
    pp.makeAbsolute(F("/config"));
    File file = fs->open(pp.get(),F("w"));
    for(const auto& it: fileContent){
        file.print(it.first);
        file.print("=");
        file.println(it.second);
    }
    file.close();
}

void ConfigFile::addConfigParameter(const String& key, const String& val) {
    fileContent.insert_or_assign(key, val);
}

bool ConfigFile::hasKey(const String& key) const {
    return fileContent.find(key) != fileContent.end();
}

const String& ConfigFile::getKey(const String& key) const {
    if (!hasKey(key))
        return emptyString;
    return fileContent.at(key);
}

void ConfigFile::clear() {
    fileContent.clear();
}

}// namespace obd::file
