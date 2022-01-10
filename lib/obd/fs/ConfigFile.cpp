/**
 * @file ConfigFile.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "ConfigFile.h"
#include "FileSystem.h"
#include "Path.h"
#include "native/fakeArduino.h"
#include <algorithm>
#include "data/DataUtils.h"

namespace obd::fs {


bool ConfigFile::configExists(const std::string& driverName) const {
    if (fs == nullptr)
        return false;
    fs::Path pp{driverName + F(".cfg")};
    pp.makeAbsolute(Path(F("/config")));
    return fs->exists(pp);
}

void ConfigFile::loadConfig(const std::string& driverName) {
    if (!configExists(driverName))
        return;
    Path configFilePath{driverName + F(".cfg")};
    configFilePath.makeAbsolute(Path(F("/config")));
    fs::TextFile file(fs.get(), configFilePath);
    while (file.available()) {
        std::string line = file.readLine();
        data::ReplaceAll(line, "\r", "");// just in case of damned Windows format
        if (line.empty())          // empty line
            continue;
        if (line[0] == '#')// comment line
            continue;
        auto idx = line.find('#');
        if (idx > 0) {// remove end line comments
            line = line.substr(0, idx);
        }
        data::ReplaceAll(line, " ", "");// remove all spaces
        idx = line.find('=');
        if (idx == std::string::npos)// invalid line
            continue;
        addConfigParameter(line.substr(0, idx), line.substr(idx + 1));
    }
    file.close();
}

void ConfigFile::saveConfig(const std::string& driverName) const {
    if (fs == nullptr)
        return;
    if (fileContent.empty())
        return;
    Path configFilePath{driverName + F(".cfg")};
    configFilePath.makeAbsolute(Path(F("/config")));
    fs::TextFile file(fs.get(), configFilePath, ios::out);
    for (const auto& it : fileContent) {
        file.write(it.first+"="+it.second+"\n");
    }
    file.close();
}

void ConfigFile::addConfigParameter(const std::string& key, const std::string& val) {
    fileContent.insert_or_assign(key, val);
}

bool ConfigFile::hasKey(const std::string& key) const {
    return fileContent.find(key) != fileContent.end();
}

const std::string& ConfigFile::getKey(const std::string& key) const {
    if (!hasKey(key))
        return data::emptyString;
    return fileContent.at(key);
}

void ConfigFile::clear() {
    fileContent.clear();
}

}// namespace obd::fs
