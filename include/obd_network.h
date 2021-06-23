/**
 * @author Silmaen
 * @date 26/05/2021
 */
#pragma once

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <obd_basedriver.h>
#include <obd_system_cmd.h>
#include <queue>

namespace obd::network {

/**
 * @brief list of the different Status of the network
 */
enum class Status {
    Disabled,       ///< no connexion, no try to connect (LED Off)
    Connecting,     ///< trying to connect to an hotspot (LED faster blinking)
    Connected,      ///< network connected to an hotspot (LED blinking)
    ConnectedClient,///< Connected to an hotspot and a client is connected to telnet server (LED Solid)
    Hotspot,        ///< this device acts as an hotspot (LED 2-blink)
    HotspotClient,  ///< this device acts as an hotspot and a client is connected to telnet server (LED 3-blink)
};

/**
 * @brief network driver
 */
class net_driver : public core::baseDriver {
public:
    /**
     * @brief Constructor with parent
     * @param p the parent system
     */
    explicit net_driver(core::system* p = nullptr);

    /**
     * @brief initialize file system
     */
    void init() override;

    /**
     * @brief print the network infos
     */
    void printInfo() override;

    /**
     * @brief procedure called each frame
     * @param delta the delta from last update
     */
    void update(int64_t delta) override;

    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    bool treatCommand(const core::command& cmd) override;

    /**
     * @brief display help on commands
     */
    void printHelp() override;

    /**
     * @brief get the name of the driver
     * @return driver name
     */
    [[nodiscard]] String getName() const override { return F("Network"); }

    /**
     * @brief attach a new parent to this driver
     * @param p the parent
     */
    void attachParent(core::system* p) override;

    /**
     * @brief load and apply parameters in the config file
     */
    void loadConfigFile() override;

    /**
     * @brief save the driver parameter in file
     */
    void saveConfigFile() const override;

    /**
     * @brief return the actual status of network
     * @return the status
     */
    [[nodiscard]] Status getCurrentStatus() const { return currentStatus; }

private:
    /// direct link to the status led
    std::shared_ptr<core::StatusLed> statusLed = nullptr;

    /// the status of the network
    Status currentStatus = Status::Disabled;

    /// the telnet server
    WiFiServer telnetServer{23};

    /// the wifi client
    WiFiClient client;

    /**
     * @brief display network status
     */
    void printStatus();

    /**
     * @brief print welcome message
     */
    void printWelcome();

    /**
     * @brief get network information and actualise the status
     * @return returns true if the status change since previous update
     */
    bool updateStatus();

    /**
     * @brief called when status change to start/stop server according to connexion
     */
    void updateServerState();

    /**
     * @brief update the connexion to client
     * @return true if something change
     */
    bool updateClientConnexion();

    /**
     * @brief listen the telnet port for instructions
     */
    void listenTelnet();

    /**
     * @brief update the LED with the status
     */
    void updateLED();
};

}// namespace obd::network
