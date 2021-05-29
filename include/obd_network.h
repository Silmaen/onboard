/**
 * \author argawaen 
 * \date 26/05/2021
 *
 */

#pragma once
#include <obd_basedriver.h>
#include <obd_system_cmd.h>
#include <queue>

namespace obd {
namespace network {

/**
 * @brief network driver
 */
class driver: public core::baseDriver {
public:
    explicit driver(core::system *p = nullptr) : baseDriver(p){}
    ~driver() = default;
    driver(const driver &) = default;
    driver(driver &&) = default;
    driver &operator=(const driver &) = default;
    driver &operator=(driver &&) = default;
    /**
     * @brief initialize file system
     */
    void init() override;

    /**
     * @brief print the network infos
     */
    void printInfo() override;

    /**
     * @brief listen to network for commands
     */
    void update() override;
    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    bool treatCommand(const core::command& cmd) override;
private:
};

}// namespace network
}// namespace obd
