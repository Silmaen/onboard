/**
 * @author argawaen
 * @date 20/06/2021
 */

#pragma once
#include "obd_basedriver.h"
#include <ESP8266WebServer.h>

namespace obd::webserver {

/**
 * @brief base driver for webserver management
 */
class driver: public core::baseDriver {
public:

    /**
     * @brief Constructor with parent
     * @param p the parent system
     */
    explicit driver(core::system* p = nullptr) :
    baseDriver(p){};

    /**
     * @brief retrieve the name of the driver
     * @return name of the driver
     */
    [[nodiscard]] String getName() const override{
        return F("Webserver");
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
    std::shared_ptr<filesystem::driver> fs = nullptr;

    void replyOK();

    void replyOKWithMsg(const String& msg);

    void replyNotFound(const String& msg) ;

    void replyBadRequest(const String& msg) ;

    void replyServerError(const String& msg);

    /**
     * @brief handle root
     */
    void hroot();
    /**
     * @brief default uridecode
     */
    void fileFb();

    /**
     * @brief readand stream file
     * @param path the path in the url
     * @return true if the file is streamed
     */
    bool handleReadFile(const String& path);
};

}// namespace obd::webserver
