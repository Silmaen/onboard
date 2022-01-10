/**
 * @file FileSystem.h
 * @author argawaen
 * @date 06/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "File.h"
#include <vector>

/**
 * @brief Namespace for core operations
 */
namespace obd::core {

/**
 * @brief Class to handle interaction with file system
 */
class FileSystem {
public:
  /**
   * @brief Constructor
   */
  FileSystem();

  /**
   * @brief Initialize file system
   * @return True if everything is ok
   */
  bool begin();

  /**
   * @brief Close the filesystem
   */
  void end();

  /**
   * @brief Check if the path exist (either file or dir)
   * @param p The path to check.
   * @return True if the path is really present.
   */
  [[nodiscard]] bool exists(const Path &p) const;

  /**
   * @brief Check if the path exist and is a directory
   * @param p The path to check
   * @return True if exists and is directory
   */
  [[nodiscard]] bool isDir(const Path &p) const;

  /**
   * @brief Check if the path exist and is a file
   * @param p The path to check
   * @return True if exists and is file
   */
  [[nodiscard]] bool isFile(const Path &p) const;

  /**
   * @brief Create a new directory
   * @param p The new directory to create
   * @param parents If True, create also parent directories.
   * @param existsOk If false raise error if dir already exists
   * @return True if operation succeed in the given condition
   */
  [[nodiscard]] bool mkdir(const Path &p, bool parents = false,
                           bool existsOk = true);

  /**
   * @brief Create an empty file if not already exists (PArent folder must
   * exist)
   * @param p The path to the file
   * @return True if the file exists, False if not exists or if already a
   * directory
   */
  [[nodiscard]] bool touch(const Path &p) const;

  /**
   * @brief Delete a file
   * @param p The file to delete
   * @return True if operation succeed
   */
  [[nodiscard]] bool rm(const Path &p) const;

  /**
   * @brief Remove a whole directory
   * @param p The directory to remove
   * @param recursive If recursive, delete also any directory content
   * @param notExistsOk don't raise error if already not exist
   * @return True if operation succeed
   */
  [[nodiscard]] bool rmdir(const Path &p, bool recursive = true,
                           bool notExistsOk = true);

  /**
   * @brief List the content of the given directory
   * @param p The directory to list (default: current working directory)
   * @return The list of path in the directory
   */
  [[nodiscard]] std::vector<Path> listDir(const Path &p = Path("")) const;

  /**
   * @brief Get the current working directory
   * @return The current working directory
   */
  [[nodiscard]] const Path &cwd() const { return currentWorkingDir; }

  /**
   * @brief Change working directory
   * @param p The new directory
   * @return True if operation succeed
   */
  bool cd(const Path &p);

  /**
   * @brief Get information about file system
   * @return String of information
   */
  [[nodiscard]] std::string getInfos() const {
    std::stringstream oss;
    oss << "cwd: " << currentWorkingDir.toString() << std::endl;
    oss << ((initialized) ? "" : "not ") << "initialized" << std::endl;
#ifndef ARDUINO
    oss << "Native base path: " << basePath << std::endl;
#endif
    return oss.str();
  }

#ifndef ARDUINO
  /// Base path for native OS
  std::filesystem::path basePath;
  /**
   * @brief Convert to std::path the internal path
   * @param path the internal path
   * @return Corresponding std::path
   */
  [[nodiscard]] std::filesystem::path toStdPath(const Path &path) const;
  /**
   * @brief Convert std::path to internal path
   * @param path The sdt::path
   * @return Corresponding internal path
   */
  [[nodiscard]] Path toInternalPath(const std::filesystem::path &path) const;
#endif

private:
  /// Current working directory
  Path currentWorkingDir;
  /// If the filesystem is initialized
  bool initialized = false;
};

} // namespace cpt::core

extern obd::core::FileSystem fs;
