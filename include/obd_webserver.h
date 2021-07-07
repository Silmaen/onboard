/**
 * @author Silmaen
 * @date 20/06/2021
 */
#pragma once

#include "obd_basedriver.h"
#include <ESP8266WebServer.h>

namespace obd::network {

/**
 * @brief base driver for webserver management
 */
class WebServer : public core::BaseDriver {
public:
    /**
     * @brief Constructor with parent
     * @param p the parent system
     */
    explicit WebServer(core::System* p = nullptr) :
        BaseDriver(p){};

    /**
     * @brief retrieve the name of the driver
     * @return name of the driver
     */
    [[nodiscard]] core::DriverType getType() const override {
        return core::DriverType::WebServer;
    }

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
     * @param delta the delta from last update
     */
    void update(int64_t delta) override;

    /**
     * @brief try to treat the given command
     * @param cmd the command to treat
     * @return true if the command has been treated
     */
    bool treatCommand([[maybe_unused]] const core::command& cmd) override;

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
    /// The web server
    ESP8266WebServer server{80};

    /// link to the filesystem
    std::shared_ptr<file::FileSystem> fs = nullptr;

    /**
     * @brief send a message with 'not Found' error code
     * @param msg the message to reply
     */
    void replyNotFound(const String& msg);

    /**
     * @brief default uri decode
     */
    void fileFb();

    /**
     * @brief take the string and replace the keywords in it.
     * @param toParse the String to transform
     */
    void StrParse(String& toParse);

    /**
     * @brief read and stream file
     * @param path the path in the url
     * @return true if the file is streamed
     */
    bool handleReadFile(const String& path);
};

}// namespace obd::network
