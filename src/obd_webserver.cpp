/**
 * @author argawaen
 * @date 20/06/2021
 */

#include "obd_webserver.h"
#include "obd_system.h"
#include "obd_filesystem.h"

namespace obd::webserver {


void driver::init() {
    if (getParent() != nullptr)
        fs = getParent()->getDriverAs<filesystem::driver>(F("FileSystem"));
    server.onNotFound([=](){this->fileFb();}); // fallback if not an previous url
    server.begin();
}

void driver::printInfo() {
}

void driver::update(int64_t delta) {
    server.handleClient();
}

bool driver::treatCommand(const core::command& cmd) {
    return baseDriver::treatCommand(cmd);
}

void driver::printHelp() {
}

void driver::loadConfigFile() {
}

void driver::saveConfigFile() const {
}

void driver::replyOK() {
    server.send(200, F("text/plain"), "");
}

void driver::replyOKWithMsg(const String& msg) {
    server.send(200, F("text/plain"), msg);
}

void driver::replyNotFound(const String& msg) {
    server.send(404, F("text/plain"), msg);
}

void driver::replyBadRequest(const String& msg) {
    getParentPrint()->println(msg);
    server.send(400, F("text/plain"), msg + "\r\n");
}

void driver::replyServerError(const String& msg) {
    getParentPrint()->println(msg);
    server.send(500, F("text/plain"), msg + "\r\n");
}

void driver::hroot() {
    char temp[400];
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;

    snprintf(temp, 400,

             "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

             hr, min % 60, sec % 60
    );
    server.send(200, "text/html", temp);
}

void driver::fileFb() {
    String uri = ESP8266WebServer::urlDecode(server.uri()); // required to read paths with blanks
    if (handleReadFile(uri)) {
        return;
    }

    // Dump debug data
    String message;
    message.reserve(100);
    message = F("Error: File not found\n\nURI: ");
    message += uri;
    message += F("\nMethod: ");
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += F("\nArguments: ");
    message += server.args();
    message += '\n';
    for (uint8_t i = 0; i < server.args(); i++) {
        message += F(" NAME:");
        message += server.argName(i);
        message += F("\n VALUE:");
        message += server.arg(i);
        message += '\n';
    }
    message += "path=";
    message += server.arg("path");
    message += '\n';

    return replyNotFound(message);
}

bool driver::handleReadFile(const String& path) {
    if (fs == nullptr) {
        getParentPrint()->println(F("Webserver: no filesystem"));
        return false;
    }
    String toDisplay{path};
    if (toDisplay.endsWith("/")) {
        toDisplay += "index.htm";
    }
    String contentType = mime::getContentType(toDisplay);
    toDisplay = config::baseWeb + toDisplay;
    if (!fs->exists(toDisplay)) { // let's try in gz format
        toDisplay += ".gz";
    }
    if (!fs->exists(toDisplay)) {
        getParentPrint()->println(F("Webserver: no file '") + toDisplay + F("'"));
        return false;
    }
    File file = fs->open(toDisplay, F("r"));
    server.streamFile(file, contentType);
    file.close();
    return true;
}

}// namespace obd::webserver
