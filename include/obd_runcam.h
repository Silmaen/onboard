/**
 * @author Silmaen
 * @date 01/07/2021.
 */

#pragma once
#include "obd_basedriver.h"
#include <SoftwareSerial.h>
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
    void update([[maybe_unused]] int64_t delta) override;

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

    enum struct RunCamControlCommand {
        RCDEVICE_PROTOCOL_SIMULATE_WIFI_BTN      = 0x00, ///< Simulation Click the Wi-Fi button
        RCDEVICE_PROTOCOL_SIMULATE_POWER_BTN     = 0x01, ///< Simulation Click the Power button
        RCDEVICE_PROTOCOL_CHANGE_MODE            = 0x02, ///< Switch the camera mode
        RCDEVICE_PROTOCOL_CHANGE_START_RECORDING = 0x03, ///< Control the camera to start recording
        RCDEVICE_PROTOCOL_CHANGE_STOP_RECORDING  = 0x04, ///< Control the camera to stop recording
    };

    /**
     * @brief send a control action to the device
     * @param command the command to send
     */
    void CameraControl(const RunCamControlCommand& command);

    /**
     * @brief the 5 possible action of the pad
     */
    enum struct RunCam5keyControl {
        RCDEVICE_PROTOCOL_5KEY_SIMULATION_SET   = 0x01, ///< Simulate the confirmation key of the 5 key remote control
        RCDEVICE_PROTOCOL_5KEY_SIMULATION_LEFT  = 0x02, ///< Simulate the left key of the 5 key remote control
        RCDEVICE_PROTOCOL_5KEY_SIMULATION_RIGHT = 0x03, ///< Simulate the right key of the 5 key remote control
        RCDEVICE_PROTOCOL_5KEY_SIMULATION_UP    = 0x04, ///< Simulate the up key of the 5 key remote control
        RCDEVICE_PROTOCOL_5KEY_SIMULATION_DOWN  = 0x05, ///< Simulate the down key of the 5 key remote control
    };

    /**
     * @brief Send the device command simulating action on 5 key pad
     * @param command the command to send
     */
    void simulate5keyRemoteControl(const RunCam5keyControl& command);

private:
    /**
     * @brief list of RunCam device protocol supported functions
     */
    enum struct RunCamCommand {
        RCDEVICE_PROTOCOL_COMMAND_GET_DEVICE_INFO = 0x00, ///< request device info
        RCDEVICE_PROTOCOL_COMMAND_CAMERA_CONTROL  = 0x01, ///< Send Camera action command
        RCDEVICE_PROTOCOL_COMMAND_5KEY_SIMULATION_PRESS = 0x02, ///< Send the Press event of the 5 key remote control to the camera
    };

    /**
     * @brief information about camera
     */
    struct RunCamInformation {
        uint8_t ProtocolVersion                                 = 0; ///< version of the protocol
        bool RCDEVICE_PROTOCOL_FEATURE_SIMULATE_POWER_BUTTON    = false; ///< if the defice can simultate power button
        bool RCDEVICE_PROTOCOL_FEATURE_SIMULATE_WIFI_BUTTON     = false; ///< if the device can simulate wifi button
        bool RCDEVICE_PROTOCOL_FEATURE_CHANGE_MODE              = false; ///< if the device can change camera mode
        bool RCDEVICE_PROTOCOL_FEATURE_SIMULATE_5_KEY_OSD_CABLE = false; ///< if the device can simulate the 5-key pad
        bool RCDEVICE_PROTOCOL_FEATURE_DEVICE_SETTINGS_ACCESS   = false; ///< if the device gives access to settings
        bool RCDEVICE_PROTOCOL_FEATURE_DISPLAYP_PORT            = false; ///< The device is identified as a DisplayPort device by flying controller and receives the OSD data display from the flight controller
        bool RCDEVICE_PROTOCOL_FEATURE_START_RECORDING          = false; ///< Control the camera to start recording video
        bool RCDEVICE_PROTOCOL_FEATURE_STOP_RECORDING           = false; ///< Control the camera to stop recording video
        bool RCDEVICE_PROTOCOL_FEATURE_FC_ATTITUDE              = false; ///< If the device support requests attitude of the remote device(like Betaflight flight controller), it should contain this flag when initializing on the remote device.
    } DeviceInfo;

    /// if the device is connected
    bool isConnected = false;

    /// if we need some debug prints
    bool debugPrint = true;

    /// current message crc
    uint8_t current_crc = 0;

    /// internal chronometer
    uint64_t chrono = 0;

    /// connexion
    SoftwareSerial uart{D5, D6};

    /**
     * @brief send command with its parameters, wait for response
     * @param cmd the command to send
     * @param params the list of parameter
     * @param expectResponse if the command will receive a response
     * @return the content of the response
     */
    std::vector<uint8_t> sendCommand(RunCamCommand cmd, const std::vector<uint8_t>& params, bool expectResponse = true);

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

    /**
     * @brief Print a bool value
     * @param ptr the bool value
     */
    void printBool(bool ptr);

    /**
     * @brief parse a command string into an instruction and send it to camera
     * @param cmd the command string to parse
     */
    void parseCmd(const String& cmd);

    /**
     * @brief parse a command string into an 5key pad and send it to camera
     * @param cmd the command string to parse
     */
    void parse5Key(const String& cmd);
};

}// namespace obd::video
