/**
 * @file Path.cpp
 * @author argawaen
 * @date 06/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "Path.h"
#include <iostream>
#include <vector>

namespace obd::fs {

/**
 * @brief Split string
 * @param x The string to split
 * @param delimiter The delimiter used to split
 * @return The string components
 */
std::vector<std::string> split(const std::string& x,
                               const std::string& delimiter) {
    std::vector<std::string> strings;
    std::string::size_type pos  = 0;
    std::string::size_type prev = 0;
    while ((pos = x.find(delimiter, prev)) != std::string::npos) {
        if (pos != prev)// no void items
            strings.push_back(x.substr(prev, pos - prev));
        prev = pos + 1;
    }
    strings.push_back(x.substr(prev));
    return strings;
}

/**
 * @brief Merge a list of strings
 * @param from Begin iterator in the string list
 * @param to End iterator in the string list
 * @param delimiter The delimiter to add between the parts
 * @return The merged string
 */
std::string merge(const std::vector<std::string>::const_iterator& from,
                  const std::vector<std::string>::const_iterator& to,
                  const std::string& delimiter) {
    if (from == to)
        return "";
    auto it(from);
    std::string result = *it;
    ++it;
    for (; it != to; ++it) {
        result += delimiter + *it;
    }
    return result;
}

/**
 * @brief Merge a list of strings
 * @param strings The list of string
 * @param delimiter The delimiter to add between the parts
 * @return The merged string
 */
std::string merge(const std::vector<std::string>& strings,
                  const std::string& delimiter) {
    return merge(strings.begin(), strings.end(), delimiter);
}

[[nodiscard]] bool Path::isAbsolute() const { return internalPath[0] == '/'; }

bool Path::isRoot() const { return internalPath == "/"; }

void Path::makeRelative(const Path& relPath) {
    if (!isAbsolute() || !relPath.isAbsolute())
        return;
    if (relPath.internalPath == "/") {
        internalPath.erase(internalPath.begin());
        return;
    }
    auto items     = split(internalPath, "/");
    auto relatives = split(relPath.internalPath, "/");
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
    internalPath = merge(items, "/");
    compact();
}

void Path::makeAbsolute(const Path& CurrentPath) {
    if (isAbsolute())
        return;
    internalPath = CurrentPath.internalPath + "/" + internalPath;
    compact();
}

Path& Path::operator/=(const Path& next) {
    internalPath += "/" + next.internalPath;
    compact();
    return *this;
}

Path& Path::operator/=(const std::string& next) {
    internalPath += "/" + next;
    compact();
    return *this;
}

Path Path::operator/(const Path& next) const {
    Path res{*this};
    res /= next;
    return res;
}

Path Path::operator/(const std::string& next) const {
    Path res{*this};
    res /= next;
    return res;
}

void Path::compact() {
    if (isRoot())
        return;
    auto items = split(internalPath, "/");
    std::vector<std::string> filtered;
    std::vector<std::string>::size_type min_id = 0;
    bool increment                             = !isAbsolute();
    for (auto& i : items) {
        if (i == "." && !(increment && filtered.empty()))
            continue;
        if (i == ".." && !increment) {
            if (filtered.size() > min_id)
                filtered.pop_back();
            continue;
        }
        if (increment) {
            if (i == ".." || i == ".") {
                ++min_id;
            } else {
                increment = false;
            }
        }
        filtered.push_back(i);
    }
    if (isAbsolute())
        internalPath = "/";
    else
        internalPath = "";
    internalPath += merge(filtered, "/");
    if (internalPath.empty())
        internalPath = ".";
}

Path Path::parent() const {
    if (isRoot())
        return {};
    auto items = split(internalPath, "/");
    if (items.size() == 1) {
        if (isAbsolute())
            return {};
        else
            return {*this};
    }
    Path result;
    if (!isAbsolute())
        result.internalPath = "";
    result.internalPath += merge(items.begin(), items.end() - 1, "/");
    return result;
}

std::string Path::name() const { return split(internalPath, "/").back(); }

std::string Path::baseName() const {
    std::string _name = name();
    return split(_name, ".").front();
}

std::string Path::suffix() const {
    std::string _name = name();
    if (_name.find('.') == std::string::npos)
        return "";
    return "." + split(_name, ".").back();
}

std::string Path::suffixes() const {
    std::string _name = name();
    if (_name.find('.') == std::string::npos)
        return "";
    auto items = split(_name, ".");
    return "." + merge(items.begin() + 1, items.end(), ".");
}

#ifndef ARDUINO

std::filesystem::path Path::toStdPath() const {
    auto items = split(internalPath, "/");
    std::filesystem::path result;
    for (const auto& it : items) {
        result /= it;
    }
    return result;
}
#endif

}// namespace obd::fs
