/**
 * @author Silmaen
 * @date 07/06/2021
 */

#include "obd_configfile.h"
#include <obd_filesystem.h>
#include <obd_system.h>

namespace obd::filesystem {

configFile::configFile(core::system* p) {
    if (p == nullptr)
        return;
    fs = p->getDriverAs<filesystem::driver>("FileSystem");
}

bool configFile::configExists(const String& driverName) const {
    if (fs == nullptr)
        return false;
    path pp{driverName + F(".cfg")};
    pp.makeAbsolute(F("/config"));
    return fs->exists(pp.get());
}

void configFile::loadConfig(const String& driverName) {
    if (!configExists(driverName))
        return;
    path pp{driverName + F(".cfg")};
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

void configFile::saveConfig(const String& driverName) const {
    if (fs == nullptr)
        return;
    if (fileContent.empty())
        return;
    path pp{driverName + F(".cfg")};
    pp.makeAbsolute(F("/config"));
    File file = fs->open(pp.get(),F("w"));
    for(auto& it: fileContent){
        file.print(it.first);
        file.print("=");
        file.println(it.second);
    }
    file.close();
}

void configFile::addConfigParameter(const String& key, const String& val) {
    fileContent.insert_or_assign(key, val);
}

bool configFile::hasKey(const String& key) const {
    return fileContent.find(key) != fileContent.end();
}

const String& configFile::getKey(const String& key) const {
    if (!hasKey(key))
        return emptyString;
    return fileContent.at(key);
}

void configFile::clear() {
    fileContent.clear();
}

}// namespace obd::filesystem
