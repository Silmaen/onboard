/**
 * @author Silmaen
 * @date 26/05/2021
 */

#include <obd_configfile.h>
#include <obd_system.h>
#include <obd_wifidriver.h>

namespace obd::network {

WifiDriver::WifiDriver(core::System* p) :
    BaseDriver(p) {
    if (p != nullptr) {
        statusLed = getParent()->getDriver<core::StatusLed>();
    }
}

void WifiDriver::init() {
    if (statusLed != nullptr)
        statusLed->setState(core::LedState::FasterBlink);
    WiFi.mode(WIFI_STA);
    WiFi.hostname(config::defaultHostname);
    loadConfigFile();
    WiFi.begin();
}

void WifiDriver::printInfo() {
    if (getParentPrint() == nullptr)
        return;
    println(F(" ----- NETWORK INFORMATION -----"));
    if (getParentPrint() != nullptr)
        WiFi.printDiag(*getParentPrint());
    const String modes[] = {
            F("Off"),
            F("Station"),
            F("Access Point"),
            F("Both")};
    print(F("Operation Mode    : "));
    println(modes[wifi_get_opmode()]);
    const String phyModes[] = {
            F(""),
            F("b"),
            F("g"),
            F("n")};
    print(F("PHY mode          : 802.11"));
    println(phyModes[static_cast<int>(wifi_get_phy_mode())]);

    // access point Infos
    if ((wifi_get_opmode() == 2) || (wifi_get_opmode() == 3))
        println(F("Access point information"));
    if ((wifi_get_opmode() == 1) || (wifi_get_opmode() == 3)) {
        // station Infos
        println(F("Station information"));
        print(F("Access point id   : "));
        println(wifi_station_get_current_ap_id());
        print(F("Access point SSID : "));
        println(WiFi.SSID());
        print(F("Channel           : "));
        println(WiFi.channel());
        print(F("Connexion Status  : "));
        String connStatus[] = {
                F("idle"),
                F("connecting"),
                F("Wrong Password"),
                F("No AP found"),
                F("Connect fail"),
                F("got IP")};
        println(connStatus[static_cast<int>(wifi_station_get_connect_status())]);
        print(F("MAC address       : "));
        println(WiFi.macAddress());
        print(F("hostname          : "));
        println(WiFi.hostname());
        if (WiFi.status() == WL_CONNECTED) {
            print(F("IP address        : "));
            println(WiFi.localIP().toString());
            print(F("Net Mask          : "));
            println(WiFi.subnetMask().toString());
            print(F("Gateway           : "));
            println(WiFi.gatewayIP().toString());
            print(F("Dns               : "));
            println(WiFi.dnsIP().toString());
        }
    }
}

void WifiDriver::update(int64_t /*delta*/) {
    if (updateStatus()) {
        updateLED();
        // do nothing more if status changed
        return;
    }
    // see if there is a new client or a client quit
    if (updateClientConnexion())
        return;
    // listen to the telnet server
    listenTelnet();
}

bool WifiDriver::treatCommand(const core::command& cmd) {
    if (cmd.isCmd(F("netinfo"))) {
        printInfo();
        return true;
    }
    if (cmd.isCmd(F("netstat"))) {
        printStatus();
        return true;
    }
    return false;
}

void WifiDriver::printHelp() {
    if (getParentPrint() == nullptr)
        return;
    println(F("Help on network interface"));
    println(F("netinfo      print information about network interface"));
    println(F("netstat      print information about network status"));
    println();
}

bool WifiDriver::updateStatus() {
    Status cal = Status::Connecting;
    if (WiFi.status() == WL_IDLE_STATUS) {
        cal = Status::Disabled;
    } else if (WiFi.status() == WL_CONNECTED) {
        if (WiFi.getMode() == WiFiMode::WIFI_AP) {
            cal = Status::Hotspot;
            if (client && client.connected() != 0U)
                cal = Status::HotspotClient;
        } else {
            cal = Status::Connected;
            if (client && client.connected() != 0U)
                cal = Status::ConnectedClient;
        }
    }
    if (cal != currentStatus) {
        currentStatus = cal;
        // start/stop the telnet server
        updateServerState();
        return true;
    }
    return false;
}

void WifiDriver::updateServerState() {
    if (currentStatus == Status::Connected || currentStatus == Status::Hotspot) {
        getParent()->getOutput()->removePrint(&client);
        if (telnetServer.status() == 0U) {
            telnetServer.begin();
            telnetServer.setNoDelay(true);
        }
    }
    if ((currentStatus == Status::Disabled || currentStatus == Status::Connecting) && telnetServer.status() != 0U) {
        getParent()->getOutput()->removePrint(&client);
        telnetServer.stop();
    }
}

bool WifiDriver::updateClientConnexion() {
    if (telnetServer.hasClient() && (!client || (client.connected() == 0U))) {
        if (client) {
            client.stop();
        }
        client = telnetServer.available();
        client.flush();
        getParent()->getOutput()->addPrint(&client);
        printWelcome();
        // do nothing more if client just connect
        return true;
    }
    return false;
}

void WifiDriver::updateLED() {
    if (statusLed == nullptr)
        return;
    switch (currentStatus) {
    case Status::Disabled:
        statusLed->setState(core::LedState::Off);
        break;
    case Status::Connecting:
        statusLed->setState(core::LedState::FasterBlink);
        break;
    case Status::Connected:
        statusLed->setState(core::LedState::Blink);
        break;
    case Status::ConnectedClient:
        statusLed->setState(core::LedState::Solid);
        break;
    case Status::Hotspot:
        statusLed->setState(core::LedState::TwoPulse);
        break;
    case Status::HotspotClient:
        statusLed->setState(core::LedState::ThreePulses);
        break;
    }
}

void WifiDriver::listenTelnet() {
    if (currentStatus != Status::HotspotClient && currentStatus != Status::ConnectedClient)
        return;

    if (client.available() > 0) {
        delay(10);
        core::command cmd(source::TELNET);
        while (client.available() > 0) {
            char c = static_cast<char>(client.read());
            if (c == '\n') break;
            if (c == '\r') break;
            if (!cmd.putChar(c)) break;
        }
        if (cmd.empty())
            return;
        pushCommand(cmd);
    }
}

void WifiDriver::printStatus() {
    if (getParent() == nullptr)
        return;
    print(F("Network status:    "));
    switch (currentStatus) {
    case Status::Disabled:
        println(F("Disabled"));
        break;
    case Status::Connecting:
        println(F("Connecting"));
        break;
    case Status::Connected:
        println(F("Station waiting for a client"));
        break;
    case Status::ConnectedClient:
        println(F("Station with a client"));
        break;
    case Status::Hotspot:
        println(F("Hotspot waiting for client"));
        break;
    case Status::HotspotClient:
        println(F("hotspot with a client"));
        break;
    }
}

void WifiDriver::printWelcome() {
    client.println();
    client.println(F("Welcome on board!"));
    client.println(F("  _______         ______                      __ "));
    client.println(F(" |       |.-----.|   __ \\.-----.---.-.----.--|  |"));
    client.println(F(" |   -   ||     ||   __ <|  _  |  _  |   _|  _  |"));
    client.println(F(" |_______||__|__||______/|_____|___._|__| |_____|"));
    client.println(F("--------------------------------------------------"));
    client.print(F(" obd version: "));
    client.print(config::version);
    client.print(F(" Author: "));
    client.println(config::author);
    client.println(F("--------------------------------------------------"));
}

void WifiDriver::loadConfigFile() {
    file::ConfigFile local_file(getParent());
    local_file.loadConfig(getName());
    // parameters to load:
    if (local_file.hasKey("host")) {
        WiFi.hostname(local_file.getKey("host").c_str());
    }
}

void WifiDriver::saveConfigFile() const {
    file::ConfigFile local_file(getParent());
    // parameter to save
    local_file.addConfigParameter("host", WiFi.hostname());
    //
    local_file.saveConfig(getName());
}

}// namespace obd::network
