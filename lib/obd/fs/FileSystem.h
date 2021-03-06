/**
 * @file FileSystem.h
 * @author argawaen
 * @date 06/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "File.h"
#include "core/driver/Node.h"
#include <utility>
#include <vector>

namespace obd::time {
class Clock;
}

/**
 * @brief Namespace for core operations
 */
namespace obd::fs {

/**
 * @brief Class to handle interaction with file system
 */
class FileSystem : public core::driver::Node, public std::enable_shared_from_this<FileSystem> {
public:
    /**
     * @brief Constructor with parent
     * @param parent The parent system
     */
    explicit FileSystem(std::shared_ptr<Messenger> parent) :
        Node(std::move(parent)), currentWorkingDir(Path("/")) {}

    /**
   * @brief Initialize file system
   */
    void init() override;

    /**
   * @brief Close the filesystem
   */
    void terminate() override;

    /**
   * @brief Check if the path exist (either file or dir)
   * @param path The path to check.
   * @return True if the path is really present.
   */
    [[nodiscard]] bool exists(const Path& path) const;

    /**
   * @brief Check if the path exist and is a directory
   * @param path The path to check
   * @return True if exists and is directory
   */
    [[nodiscard]] bool isDir(const Path& path) const;

    /**
   * @brief Check if the path exist and is a file
   * @param path The path to check
   * @return True if exists and is file
   */
    [[nodiscard]] bool isFile(const Path& path) const;

    /**
   * @brief Create a new directory
   * @param path The new directory to create
   * @param parents If True, create also parent directories.
   * @param existsOk If false raise error if dir already exists
   * @return True if operation succeed in the given condition
   */
    [[nodiscard]] bool mkdir(const Path& path, bool parents = false,
                             bool existsOk = true);

    /**
   * @brief Create an empty file if not already exists (parent folder must
   * exist)
   * @param path The path to the file
   * @return True if the file exists, False if not exists or if already a
   * directory
   */
    [[nodiscard]] bool touch(const Path& path);

    /**
   * @brief Delete a file
   * @param path The file to delete
   * @return True if operation succeed
   */
    [[nodiscard]] bool rm(const Path& path) const;

    /**
   * @brief Remove a whole directory
   * @param path The directory to remove
   * @param recursive If recursive, delete also any directory content
   * @param notExistsOk don't raise error if already not exist
   * @return True if operation succeed
   */
    [[nodiscard]] bool rmdir(const Path& path, bool recursive = true,
                             bool notExistsOk = true);

    /**
   * @brief List the content of the given directory
   * @param path The directory to list (default: current working directory)
   * @return The list of path in the directory
   */
    [[nodiscard]] std::vector<Path> listDir(const Path& path = Path("")) const;

    /**
   * @brief Get the current working directory
   * @return The current working directory
   */
    [[nodiscard]] const Path& cwd() const { return currentWorkingDir; }

    /**
   * @brief Change working directory
   * @param path The new directory
   * @return True if operation succeed
   */
    bool cd(const Path& path);

#ifndef ARDUINO
    /**
   * @brief Convert to std::path the internal path
   * @param path the internal path
   * @return Corresponding std::path
   */
    [[nodiscard]] std::filesystem::path toStdPath(const Path& path) const;
    /**
   * @brief Convert std::path to internal path
   * @param path The sdt::path
   * @return Corresponding internal path
   */
    [[nodiscard]] Path toInternalPath(const std::filesystem::path& path) const;
#endif
    /**
     * @brief Try to link the given node
     * @param node The node to link to this one
     * @return True if linked
     */
    bool linkNode(const std::shared_ptr<Node>& node) override;

private:
    /// Current working directory
    Path currentWorkingDir;
    /// lin to the clock
    std::shared_ptr<time::Clock> clock = nullptr;
#ifndef ARDUINO
    /// Base path for native OS
    std::filesystem::path basePath;
#endif

    /**
     * @brief Get information about file system
     * @return String of information
     */
    [[nodiscard]] OString info() const override;

    /**
     * @brief Set time callback
     * @param callBack The time call back
     */
    void setTimeCb();
};

}// namespace obd::fs
