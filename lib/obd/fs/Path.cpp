/**
 * @file Path.cpp
 * @author argawaen
 * @date 06/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "Path.h"
#include "data/DataUtils.h"

namespace obd::fs {

[[nodiscard]] bool Path::isAbsolute() const { return internalPath[0] == '/'; }

bool Path::isRoot() const { return internalPath == "/"; }

void Path::makeRelative(const Path& relPath) {
    if (!isAbsolute() || !relPath.isAbsolute())
        return;
    if (relPath.internalPath == "/") {
        internalPath.erase(internalPath.begin());
        return;
    }
    auto items     = data::split(internalPath, "/");
    auto relatives = data::split(relPath.internalPath, "/");
    bool start     = true;
    for (const auto& r : relatives) {
        if (start) {
            if (items.front() == r) {
                items.erase(items.begin());
            } else {
                start = false;
                items.insert(items.begin(), "..");
            }
        } else {
            items.insert(items.begin(), "..");
        }
    }
    internalPath = data::merge(items, "/");
    compact();
}

void Path::makeAbsolute(const Path& CurrentPath) {
    if (isAbsolute())
        return;
    internalPath = CurrentPath.internalPath + "/" + internalPath;
    compact();
}

Path& Path::operator/=(const Path& next) {
    internalPath += OString("/") + next.internalPath;
    compact();
    return *this;
}

Path& Path::operator/=(const OString& next) {
    internalPath += OString("/") + next;
    compact();
    return *this;
}

Path Path::operator/(const Path& next) const {
    Path res{*this};
    res /= next;
    return res;
}

Path Path::operator/(const OString& next) const {
    Path res{*this};
    res /= next;
    return res;
}

void Path::compact() {
    if (isRoot())
        return;
    auto items = data::split(internalPath, "/");
    std::vector<OString> filtered;
    std::vector<OString>::size_type min_id = 0;
    bool increment                             = !isAbsolute();
    for (auto& item : items) {
        if (item == "." && !(increment && filtered.empty()))
            continue;
        if (item == ".." && !increment) {
            if (filtered.size() > min_id)
                filtered.pop_back();
            continue;
        }
        if (increment) {
            if (item == ".." || item == ".") {
                ++min_id;
            } else {
                increment = false;
            }
        }
        filtered.push_back(item);
    }
    if (isAbsolute()) {
        internalPath = "/";
    } else {
        internalPath = "";
    }
    internalPath += data::merge(filtered, "/");
    if (internalPath.empty())
        internalPath = ".";
}

Path Path::parent() const {
    if (isRoot())
        return {};
    auto items = data::split(internalPath, "/");
    if (items.size() == 1) {
        if (isAbsolute())
            return {};
        return {*this};
    }
    Path result;
    if (!isAbsolute())
        result.internalPath = "";
    result.internalPath += data::merge(items.begin(), items.end() - 1, "/");
    return result;
}

OString Path::name() const { return data::split(internalPath, "/").back(); }

OString Path::baseName() const {
    OString _name = name();
    return data::split(_name, ".").front();
}

OString Path::suffix() const {
    OString _name = name();
    if (_name.find('.') == OString::npos)
        return "";
    return OString(".") + data::split(_name, ".").back();
}

OString Path::suffixes() const {
    OString _name = name();
    if (_name.find('.') == OString::npos)
        return "";
    auto items = data::split(_name, ".");
    return OString(".") + data::merge(items.begin() + 1, items.end(), ".");
}

#ifndef ARDUINO

std::filesystem::path Path::toStdPath() const {
    auto items = data::split(internalPath, "/");
    std::filesystem::path result;
    for (const auto& item : items) {
        result /= item.s_str();
    }
    return result;
}
#endif

}// namespace obd::fs
