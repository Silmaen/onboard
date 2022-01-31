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
#include <iostream>
#include "data/DataUtils.h"

namespace obd::fs {


bool ConfigFile::configExists(const OString& driverName) const {
    if (fs == nullptr)
        return false;
    fs::Path pp{driverName + F(".cfg")};
    pp.makeAbsolute(Path(F("/config")));
    return fs->exists(pp);
}

void ConfigFile::loadConfig(const OString& driverName) {
    if (!configExists(driverName))
        return;
    Path configFilePath{driverName + F(".cfg")};
    configFilePath.makeAbsolute(Path(F("/config")));
    fs::TextFile file(fs, configFilePath);
    while (file.available()) {
        OString line = file.readLine(255,false);
        if (line.empty())          // empty line
            continue;
        if (line[0] == '#')// comment line
            continue;
        auto idx = line.find('#');
        if (idx > 0) {// remove end line comments
            line = line.substr(0, idx);
        }
        line = data::ReplaceAll(line, " ", "");// remove all spaces
        idx = line.find('=');
        if (idx == OString::npos)// invalid line
            continue;
        addConfigParameter(line.substr(0, idx), line.substr(idx + 1));
    }
    file.close();
}

void ConfigFile::saveConfig(const OString& driverName) const {
    if (fs == nullptr)
        return;
    if (fileContent.empty())
        return;
    Path configFilePath{driverName + F(".cfg")};
    configFilePath.makeAbsolute(Path(F("/config")));
    fs::TextFile file(fs, configFilePath, ios::out);
    file.write(OString("# Config file for driver: ") + driverName +" \n\n");
    for (const auto& parameter : fileContent) {
        file.write(parameter.first+"="+ parameter.second+"\n");
    }
    file.close();
}

void ConfigFile::addConfigParameter(const OString& key, const OString& val) {
    fileContent.insert_or_assign(key, val);
}

bool ConfigFile::hasKey(const OString& key) const {
    return fileContent.find(key) != fileContent.end();
}

const OString& ConfigFile::getKey(const OString& key) const {
    if (!hasKey(key))
        return data::emptyString;
    return fileContent.at(key);
}

void ConfigFile::clear() {
    fileContent.clear();
}

}// namespace obd::fs
