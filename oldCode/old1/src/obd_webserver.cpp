/**
 * @author Silmaen
 * @date 20/06/2021
 */

#include "obd_webserver.h"
#include "obd_filesystem.h"
#include "obd_system.h"
#include "obd_systemtime.h"

namespace obd::network {


void WebServer::init() {
    fs = getParent()->getDriver<file::FileSystem>();
    server.onNotFound([=]() { this->fileFb(); });// fallback if not an previous url
    server.begin();
}

void WebServer::printInfo() {
}

void WebServer::update(int64_t /*delta*/) {
    server.handleClient();
}

bool WebServer::treatCommand(const core::command& cmd) {
    return BaseDriver::treatCommand(cmd);
}

void WebServer::printHelp() {
}

void WebServer::loadConfigFile() {
}

void WebServer::saveConfigFile() const {
}

void WebServer::replyNotFound(const String& msg) {
    server.send(404, F("text/plain"), msg);
}

void WebServer::fileFb() {
    String uri = ESP8266WebServer::urlDecode(server.uri());// required to read paths with blanks
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
    for (int i = 0; i < server.args(); i++) {
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

bool WebServer::handleReadFile(const String& path) {
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

    if (!fs->exists(toDisplay)) {// let's try in gz format
        toDisplay += ".gz";
    }
    if (!fs->exists(toDisplay)) {
        println(F("Webserver: no file '") + toDisplay + F("'"));
        return false;
    }
    println(F("Webserver Response to : ") + path);
    File file = fs->open(toDisplay, F("r"));
    if (contentType != F("text/html")) {
        server.streamFile(file, contentType);
    } else {
        String content = file.readString();
        StrParse(content);
        server.send(200, contentType, content);
    }
    file.close();
    return true;
}

void WebServer::StrParse(String& toParse) {
    auto clock = getParent()->getDriver<time::Clock>();
    if (clock != nullptr)
        toParse.replace("{{date}}", clock->getDateFormatted());
}

}// namespace obd::network
