/**
 * @file RunCam.h
 * @author argawaen
 * @date 11/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "LorisMenu.h"
#include "core/driver/Node.h"
#ifdef ARDUINO
#include <SoftwareSerial.h>
#endif

namespace obd::camera {

/**
 * @brief Class to handle the RunCam device
 */
class RunCam : public core::driver::Node {
public:
    /**
     * @brief Possible Status of the camera
     */
    enum struct Status {
        DISCONNECTED,///< No camera connected
        READY,       ///< Camera connected, waiting
        RECORDING,   ///< Camera recording
        MENU,        ///< Navigation inside Menu
        MANUAL       ///< Manual Mode: No remote control of the camera
    };
    /**
     * @brief Constructor with parent
     * @param parent The parent system
     */
    explicit RunCam(const std::shared_ptr<Messenger>& parent = nullptr) :
        Node(parent){};

    /**
     * @brief Initialize file system
     */
    void init() override;

    /**
     * @brief Return the driver infos
     * @return The driver's infos.
     */
    [[nodiscard]] OString info()const override;

    /**
     * @brief Try to treat the given command
     * @param message The command to treat
     * @return True if the command treated
     */
    bool treatMessage(const Message& message) override;

    /**
     * @brief Retrieve infos from device
     */
    void getDeviceInfo();

    /**
     * @brief Move to manual mode
     */
    void setManual();

    /**
     * @brief Move from manual mode to READY
     */
    void unsetManual();

    /**
     * @brief Move from any mode/state to READY
     */
    void resetState ();

    /**
     * @brief Start camera recording
     */
    void startRecording();

    /**
     * @brief Stop camera recording
     */
    void stopRecording();

    /**
     * @brief Open the menu
     */
    void openMenu();

    /**
     * @brief Move left in the menu
     */
    void moveLeft();

    /**
     * @brief Move right in the menu
     */
    void moveRight();

    /**
     * @brief Do a validation action in the menu
     */
    void moveSet();

    /**
     * @brief Move up in the menu
     */
    void moveUp();

    /**
     * @brief Move down in the menu
     */
    void moveDown();

    /**
     * @brief Append some time to the chronometer
     * @param addedTime The time to add
     */
    void accelerateTime(uint64_t addedTime);

private:
    /**
     * @brief List of RunCam device protocol supported functions
     */
    enum struct Command {
        GET_DEVICE_INFO         = 0x00,///< Request device info
        CAMERA_CONTROL          = 0x01,///< Send Camera action command
        KEY5_SIMULATION_PRESS   = 0x02,///< Send the Press event of the 5 key remote control to the camera
        KEY5_SIMULATION_RELEASE = 0x03,///< Send the release event of the 5 key remote control to the camera
        KEY5_CONNECTION         = 0x04,///< Send handshake events and disconnected events to the camera
    };

    /**
     * @brief List of device features
     */
    enum struct Feature {
        SIMULATE_POWER_BUTTON    = 0x00,///< The device can simulate power button
        SIMULATE_WIFI_BUTTON     = 0x01,///< The device can simulate Wi-Fi button
        CHANGE_MODE              = 0x02,///< The device can change camera mode
        SIMULATE_5_KEY_OSD_CABLE = 0x03,///< The device can simulate the 5-key pad
        DEVICE_SETTINGS_ACCESS   = 0x04,///< The device gives access to settings
        DISPLAY_PORT             = 0x05,///< Display Port feature. Receives the OSD data display from the flight controller.
        START_RECORDING          = 0x06,///< Control the camera to start recording video
        STOP_RECORDING           = 0x07,///< Control the camera to stop recording video
        FC_ATTITUDE              = 0x09,///< Support requests attitude of the remote device.
    };

    /**
     * @brief List of base commands
     */
    enum struct ControlCommand {
        SIMULATE_WIFI_BTN      = 0x00,///< Simulation Click the Wi-Fi button
        SIMULATE_POWER_BTN     = 0x01,///< Simulation Click the Power button
        CHANGE_MODE            = 0x02,///< Switch the camera mode
        CHANGE_START_RECORDING = 0x03,///< Control the camera to start recording
        CHANGE_STOP_RECORDING  = 0x04,///< Control the camera to stop recording
    };

    /**
     * @brief Information about camera
     */
    struct RunCamInformation {
        uint8_t ProtocolVersion = 0;///< Version of the protocol
        uint16_t Features       = 0;///< List of features

        /**
         * @brief Test availability of a feature
         * @param feature The feature to request
         * @return True means feature available.
         */
        [[nodiscard]] bool hasFeature(const Feature& feature) const {
            return (Features & (1U << static_cast<uint8_t>(feature))) != 0;
        }
    } DeviceInfo;
    /// Internal timestamp
    uint64_t timestamp = 0;

    /// Current status of the camera
    Status status = Status::DISCONNECTED;

    /// If we need some debug prints
    bool debugPrint = false;

    /// Current message crc
    uint8_t current_crc = 0;

    /// internal chronometer
    uint64_t chrono = 0;


#ifdef ARDUINO
    /// connexion
    SoftwareSerial uart{D2, D1};
#endif

    /// simulation of the menu navigation
    LorisMenu navMenu;

    /**
     * @brief Listen to network for commands
     */
    void preTreatment() override;

    /**
     * @brief Send command with its parameters, wait for response
     * @param cmd The command to send
     * @param params The list of parameter
     * @param expectResponse Do we expect a response
     * @return The content of the response
     */
    std::vector<uint8_t> sendCommand(Command cmd, const std::vector<uint8_t>& params, bool expectResponse = true);

    /**
     * @brief Send a message to this driver
     * @param message The Command message to send
     * @return True mean command caught.
     */
    bool pushCommand(const Message& message)override;
    /**
     * @brief Reset the current crc code
     */
    void resetCrc() { current_crc = 0; }

    /**
     * @brief Append a char to the current crc code
     * @param addChar The char to add
     */
    void crc8_dvb_s2(uint8_t addChar) {
        current_crc ^= addChar;
        for (int ii = 0; ii < 8; ++ii) current_crc = ((current_crc & 0x80) != 0) ? (current_crc << 1) ^ 0xD5 : current_crc << 1;
    }

    /**
     * @brief Parse a command string into an instruction and send it to camera
     * @param cmd The command string to parse
     */
    void parseCmd(const OString& cmd);

    /**
     * @brief Parse a command string into a menu movement
     * @param cmd The command string to parse
     */
    void parseMenu(const OString& cmd);

    /**
     * @brief Generic function to move into the menu
     * @param dir The menu's movement direction
     */
    void moveMenu(const NavDirection& dir);
};
}// namespace obd::camera
