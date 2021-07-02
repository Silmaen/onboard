/**
 * @author Silmaen
 * @date 01/07/2021.
 */

#pragma once
#include "obd_basedriver.h"
#include <vector>

namespace obd::video {

/**
 * @brief class to handle the RunCam device
 */
class RunCam : public core::baseDriver {
public:
    /**
     * @brief constructor with parent
     * @param p the parent system
     */
    explicit RunCam(core::system* p = nullptr) :
        baseDriver(p){};

    /**
     * @brief initialize file system
     */
    void init() override;

    /**
     * @brief print the file system information in the given stream
     */
    void printInfo() override;

    /**
     * @brief listen to network for commands
     * @param delta the time delta from last update
     */
    void update([[maybe_unused]] int64_t delta) override {}

    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    bool treatCommand(const core::command& cmd) override;

    /**
     * @brief display command help
     */
    void printHelp() override;

    /**
     * @brief get the name of the driver
     * @return driver name
     */
    [[nodiscard]] String getName() const override { return F("RunCam"); }

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
     * @brief list of RunCam device protocol supported functions
     */
    enum struct RunCamCommand {
        RCDEVICE_PROTOCOL_COMMAND_GET_DEVICE_INFO = 0x00,///< request device info

    };

    struct RunCamInformation {
        uint8_t ProtocolVersion                                 = 0;
        bool RCDEVICE_PROTOCOL_FEATURE_SIMULATE_POWER_BUTTON    = false;
        bool RCDEVICE_PROTOCOL_FEATURE_SIMULATE_WIFI_BUTTON     = false;
        bool RCDEVICE_PROTOCOL_FEATURE_CHANGE_MODE              = false;
        bool RCDEVICE_PROTOCOL_FEATURE_SIMULATE_5_KEY_OSD_CABLE = false;
        bool RCDEVICE_PROTOCOL_FEATURE_DEVICE_SETTINGS_ACCESS   = false;
        bool RCDEVICE_PROTOCOL_FEATURE_DISPLAYP_PORT            = false;
        bool RCDEVICE_PROTOCOL_FEATURE_START_RECORDING          = false;
        bool RCDEVICE_PROTOCOL_FEATURE_STOP_RECORDING           = false;
        bool RCDEVICE_PROTOCOL_FEATURE_FC_ATTITUDE              = false;
    } __attribute__((aligned(16))) DeviceInfo;

    /// if the device is connected
    bool isConnected = false;

    /// current message crc
    uint8_t current_crc = 0;

    /**
     * @brief send command with its parameters, wait for response
     * @param cmd the command to send
     * @param params the list of parameter
     * @return the content of the response
     */
    std::vector<uint8_t> sendCommand(RunCamCommand cmd, const std::vector<uint8_t>& params);

    /**
     * @brief retrieve camera information throw Serial communication
     */
    void getCameraInfo();

    /**
     * @brief reset the current crc code
     */
    void resetCrc() { current_crc = 0; }

    /**
     * @brief append a char to the current crc code
     * @param a the char to add
     */
    void crc8_dvb_s2(uint8_t a) {
        current_crc ^= a;
        for (int ii = 0; ii < 8; ++ii) current_crc = ((current_crc & 0x80) != 0) ? (current_crc << 1) ^ 0xD5 : current_crc << 1;
    }
};

}// namespace obd::video
