/**
 * @file Path.h
 * @author argawaen
 * @date 06/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <string>
#include <utility>
#ifndef ARDUINO
#include <filesystem>
#endif

namespace obd::fs {

/**
 * @brief Class handling file path
 */
class Path {
public:
    /// default constructor
    Path() :
        internalPath("/") {}
    /// default assignation constructor
    Path(const Path&) = default;
    /// default copy constructor
    Path(Path&&) = default;
    /**
   * @brief Constructor using a string
   * @param path The initialization string
   */
    explicit Path(std::string&& path) :
        internalPath{std::move(path)} {}
    /**
   * @brief Affectation operator
   * @return this
   */
    Path& operator=(const Path&) = default;
    /**
   * @brief Move affectation
   * @return this
   */
    Path& operator=(Path&&) = default;
    /**
   * @brief Move affectation of string
   * @param path The original string
   * @return this
   */
    Path& operator=(std::string&& path) {
        internalPath = std::move(path);
        return *this;
    }
    /**
   * @brief Check if absolute path
   * @return True if absolute path
   */
    [[nodiscard]] bool isAbsolute() const;
    /**
   * @brief Check if the path is the root path
   * @return True if root path
   */
    [[nodiscard]] bool isRoot() const;
    /**
   * @brief Make this path relative to the given path
   * @param relPath The path to be relative to.
   */
    void makeRelative(const Path& relPath);
    /**
   * @brief Make this path as absolute
   * @param CurrentPath The current path
   */
    void makeAbsolute(const Path& CurrentPath);
    /**
   * @brief Append a path to this one
   * @param next The path to append
   * @return this
   */
    Path& operator/=(const Path& next);
    /**
   * @brief Append a path to this one
   * @param next The path to append
   * @return this
   */
    Path& operator/=(const std::string& next);
    /**
   * @brief Append a path to this one
   * @param next The path to append
   * @return The merged paths
   */
    Path operator/(const Path& next) const;
    /**
   * @brief Append a path to this one
   * @param next The path to append
   * @return The merged paths
   */
    Path operator/(const std::string& next) const;
    /**
   * @brief Compact this path (interpret the ".." & ".")
   */
    void compact();
    /**
   * @brief Get the parent Path
   * @return The parent Path
   */
    [[nodiscard]] Path parent() const;
    /**
   * @brief Get the file name with extension
   * @return File name
   */
    [[nodiscard]] std::string name() const;
    /**
   * @brief Get the file name without extensions
   * @return File name
   */
    [[nodiscard]] std::string baseName() const;
    /**
   * @brief Get the last suffix of the path
   * @return The last suffix of the path
   */
    [[nodiscard]] std::string suffix() const;
    /**
   * @brief Get all the path's suffixes
   * @return The path's suffixes
   */
    [[nodiscard]] std::string suffixes() const;
    /**
   * @brief Make this path as a string
   * @return This path
   */
    [[nodiscard]] const std::string& toString() const { return internalPath; }
#ifndef ARDUINO
    /**
   * @brief Make this path as a std::path
   * @return This path
   */
    [[nodiscard]] std::filesystem::path toStdPath() const;
#endif

private:
    /// String to store path data
    std::string internalPath;
};

}// namespace obd::fs
