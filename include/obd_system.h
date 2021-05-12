//
// Created by damien.lachouette on 10/05/2021.
//

#pragma once
#include <queue>
#include <obd_system_cmd.h>

/**
 * @brief base namespace of the project
 */
namespace obd {
/**
 * @brief namespace for the base system
 */
namespace system {

/**
 * @brief implementation of the system
 */
class impl {
public:
    /**
     * @brief initialize the system
     */
    void init();
    /**
     * @brief update the system
     */
    void update();

private :
    /**
     * @brief print the kernel information in the given stream
     * @param output the stream onto write
     */
    static void printKernelInfo(Stream *output);

    /**
     * @brief print the system information
     * @param output the stream onto write
     */
    static void printSystemInfo(Stream *output);

    /**
     * @brief load config from filesystem
     */
    bool loadConfig();

    /**
     * @brief save config to filesystem
     */
    bool saveConfig();

    /**
     * @brief treat the command queue
     */
    void treatCommands();

    /**
     * @brief list of output streams
     */
    std::vector<Stream*> currStreams;
    /**
     * @brief queue of the commands
     */
    std::queue<command> commands;
};

}// namespace system

extern system::impl hardware;

} // namespace obd
