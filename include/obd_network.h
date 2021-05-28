/**
 * \author argawaen 
 * \date 26/05/2021
 *
 */

#pragma once
#include <Print.h>
#include <obd_system_cmd.h>
#include <queue>

namespace obd {
namespace internal {

class network_impl {
public:
    /**
     * @brief initialize file system
     * @param out the print output
     */
    void init(Print* out);

    /**
     * @brief print the network infos
     */
    void printInfo();

    /**
     * @brief listen to network for commands
     * @param cmds the list of command where to append received
     */
    void update(std::queue<system::command>* cmds);
private:
    Print* output = nullptr;
};

} // namespace internal

extern internal::network_impl network;

} // namespace obd
