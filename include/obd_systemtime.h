/**
 * \author argawaen 
 * \date 08/06/2021
 *
 */

#pragma once
#include "obd_basedriver.h"
#include <ctime>
#include <Udp.h>

namespace obd::time {

/// size of a ntp packet
constexpr uint16_t NtpPacketSize=48U;
/// default local udp port number
constexpr uint16_t NtpDefaultLocalPort=1337;
/// default update interval in ms
constexpr uint64_t defaultUpdateInterval = 60000;
/// seconds between Jan 1 1900 and Jan 1 1970
constexpr time_t SevenZyYears = 2208988800UL;

class clock : public core::baseDriver {
public:
    explicit clock(core::system* p = nullptr) :
        baseDriver{p} {}

    /**
     * @brief retrieve the name of the driver
     * @return name of the driver
     */
    [[nodiscard]] String getName() const override { return F("SystemClock");}

    /**
     * @brief initialize the driver
     */
    void init() override;

    /**
     * @brief print the driver infos
     */
    void printInfo() override;

    /**
     * @brief procedure called each frame
     * @param delta the current timestamp of the device
     */
    void update(int64_t delta) override;

    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    bool treatCommand(const core::command& cmd) override;

    /**
     * @brief display driver help on commands
     */
    void printHelp() override;

    /**
     * @brief load and apply parameters in the config file
     */
    void loadConfigFile() override;

    /**
     * @brief save the driver parameter in file
     */
    void saveConfigFile() const override;
private:
    /**
     * @brief display the current date and time of the system
     */
    void printDate();
    /**
     * @brief send NTP requested to the pool
     */
    void sendNTPPacket();

    /**
     * @brief update value from NTP server
     * @return true if UDP request successfully replied
     */
    bool updateNTP();

    /**
     * @brief check if the network is in a state that allow NTP communication
     * @return true if NTP request is possible
     */
    bool checkNetworkState();

    ///link to the network driver
    network::driver* net = nullptr;
    /// port to communicate with NTP Servers
    UDP* udpConn = nullptr;
    /// the pool server name to query
    String poolServerName;
    /// port number
    uint16_t port = NtpDefaultLocalPort;
    /// chronometer in ms
    uint64_t timer = 0;
    /// update interval in ms
    uint64_t updateInterval = defaultUpdateInterval;
    /// value of time
    time_t currentEpoch;
};

}// namespace obd::time
