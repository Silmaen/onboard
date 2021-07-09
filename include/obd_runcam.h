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
class RunCam : public core::BaseDriver {
public:
    /**
     * @brief constructor with parent
     * @param p the parent system
     */
    explicit RunCam(core::System* p = nullptr) :
        BaseDriver(p){};

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
     * @brief load and apply parameters in the config file
     */
    void loadConfigFile() override;

    /**
     * @brief save the driver parameter in file
     */
    void saveConfigFile() const override;

    /**
     * @brief simulate a push on the wifi button
     */
    void simulateWifiBtn();

    /**
     * @brief simulate a push on the power button
     */
    void simulatePowerBtn();

    /**
     * @brief try to change camera mode
     */
    void cameraChangeMode();

    /**
     * @brief request a recording start
     */
    void cameraRecordingStart();

    /**
     * @brief request a recording stop
     */
    void cameraRecordingStop();

    /**
     * @brief the 5 possible action of the pad and the release.
     */
    enum struct key5Control {
        SET     = 0x01,///< Simulate the confirmation key of the 5 key remote control
        LEFT    = 0x02,///< Simulate the left key of the 5 key remote control
        RIGHT   = 0x03,///< Simulate the right key of the 5 key remote control
        UP      = 0x04,///< Simulate the up key of the 5 key remote control
        DOWN    = 0x05,///< Simulate the down key of the 5 key remote control
        RELEASE = 0x06,///< Simulate the release of the button
    };

    /**
     * @brief Send the device command simulating action on 5 key pad
     * @param command the command to send
     */
    void simulate5keyRemoteControl(const key5Control& command);

    /**
     * @brief try to handshake or disconnect device
     * @param open true: connexion, false: disconnection
     */
    void OpenClose(bool open = true);

private:
    /**
     * @brief list of RunCam device protocol supported functions
     */
    enum struct Command {
        GET_DEVICE_INFO         = 0x00,///< request device info
        CAMERA_CONTROL          = 0x01,///< Send Camera action command
        KEY5_SIMULATION_PRESS   = 0x02,///< Send the Press event of the 5 key remote control to the camera
        KEY5_SIMULATION_RELEASE = 0x03,///< Send the release event of the 5 key remote control to the camera
        KEY5_CONNECTION         = 0x04,///< Send handshake events and disconnected events to the camera
    };

    enum struct Feature {
        SIMULATE_POWER_BUTTON    = 0,///< if the defice can simultate power button
        SIMULATE_WIFI_BUTTON     = 0,///< if the device can simulate wifi button
        CHANGE_MODE              = 0,///< if the device can change camera mode
        SIMULATE_5_KEY_OSD_CABLE = 0,///< if the device can simulate the 5-key pad
        DEVICE_SETTINGS_ACCESS   = 0,///< if the device gives access to settings
        DISPLAYP_PORT            = 0,///< The device is identified as a DisplayPort device by flying controller and receives the OSD data display from the flight controller
        START_RECORDING          = 0,///< Control the camera to start recording video
        STOP_RECORDING           = 0,///< Control the camera to stop recording video
        FC_ATTITUDE              = 0,///< If the device support requests attitude of the remote device(like Betaflight flight controller), it should contain this flag when initializing on the remote device.
    };

    enum struct ControlCommand {
        SIMULATE_WIFI_BTN      = 0x00,///< Simulation Click the Wi-Fi button
        SIMULATE_POWER_BTN     = 0x01,///< Simulation Click the Power button
        CHANGE_MODE            = 0x02,///< Switch the camera mode
        CHANGE_START_RECORDING = 0x03,///< Control the camera to start recording
        CHANGE_STOP_RECORDING  = 0x04,///< Control the camera to stop recording
    };

    /**
     * @brief information about camera
     */
    struct RunCamInformation {
        uint8_t ProtocolVersion = 0;///< version of the protocol
        uint16_t Features       = 0;///< list of features

        [[nodiscard]] bool hasFeature(const Feature& f) const {
            return (Features & (1U << static_cast<uint8_t>(f))) != 0;
        }
    } DeviceInfo;

    /// if the device is connected
    bool isConnected = false;

    /// if a session to the 5 key pad is active
    bool is5keyConnected = false;

    /// if we need some debug prints
    bool debugPrint = true;

    /// current message crc
    uint8_t current_crc = 0;

    /// internal chronometer
    uint64_t chrono = 0;

    /// connexion
    SoftwareSerial uart{D5, D6};

    /**
     * @brief retrieve camera information throw Serial communication
     */
    void getCameraInfo(bool silent = true);

    /**
     * @brief send command with its parameters, wait for response
     * @param cmd the command to send
     * @param params the list of parameter
     * @param expectResponse if the command will receive a response
     * @return the content of the response
     */
    std::vector<uint8_t> sendCommand(Command cmd, const std::vector<uint8_t>& params, bool expectResponse = true, bool silent = true);


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
