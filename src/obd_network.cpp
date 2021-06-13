/**
 * @author Silmaen
 * @date 26/05/2021
 */
#include <obd_configfile.h>
#include <obd_network.h>
#include <obd_system.h>

namespace obd::network {

driver::driver(core::system* p) :
    baseDriver(p) {
    if (p != nullptr) {
        statusLed = p->getDriverAs<core::StatusLed>(F("StatusLed"));
    }
}

void driver::attachParent(core::system* p) {
    baseDriver::attachParent(p);
    if (p != nullptr) {
        statusLed = p->getDriverAs<core::StatusLed>(F("StatusLed"));
    }
}

void driver::init() {
    if (statusLed != nullptr)
        statusLed->setState(core::LedState::FasterBlink);
    WiFi.mode(WIFI_STA);
    WiFi.hostname(defaultHostname);
    loadConfigFile();
    WiFi.begin();
    if (getParentPrint() != nullptr)
        WiFi.printDiag(*getParentPrint());
}

void driver::printInfo() {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(F(" ----- NETWORK INFORMATIONS -----"));
    const String modes[] = {
            F("Off"),
            F("Station"),
            F("Access Point"),
            F("Both")};
    getParentPrint()->print(F("Operation Mode    : "));
    getParentPrint()->println(modes[wifi_get_opmode()]);
    const String phyModes[] = {
            F(""),
            F("b"),
            F("g"),
            F("n")};
    getParentPrint()->print(F("PHY mode          : 802.11"));
    getParentPrint()->println(phyModes[static_cast<int>(wifi_get_phy_mode())]);

    if ((wifi_get_opmode() == 2) || (wifi_get_opmode() == 3)) {
        // access point Infos
        getParentPrint()->println(F("Access point informations"));
    }
    if ((wifi_get_opmode() == 1) || (wifi_get_opmode() == 3)) {
        // station Infos
        getParentPrint()->println(F("Station informations"));
        getParentPrint()->print(F("Access point id   : "));
        getParentPrint()->println(wifi_station_get_current_ap_id());
        getParentPrint()->print(F("Access point SSID : "));
        getParentPrint()->println(WiFi.SSID());
        getParentPrint()->print(F("Channel           : "));
        getParentPrint()->println(WiFi.channel());
        getParentPrint()->print(F("Connexion Status  : "));
        String connStatus[] = {
                F("idle"),
                F("connecting"),
                F("Wrong Password"),
                F("No AP found"),
                F("Connect fail"),
                F("got IP")};
        getParentPrint()->println(connStatus[static_cast<int>(wifi_station_get_connect_status())]);

        getParentPrint()->print(F("MAC address       : "));
        getParentPrint()->println(WiFi.macAddress());

        getParentPrint()->print(F("hostname          : "));
        getParentPrint()->println(WiFi.hostname());
        if (WiFi.status() == WL_CONNECTED) {
            getParentPrint()->print(F("IP address        : "));
            getParentPrint()->println(WiFi.localIP().toString());
            getParentPrint()->print(F("Net Mask          : "));
            getParentPrint()->println(WiFi.subnetMask().toString());
            getParentPrint()->print(F("Gateway           : "));
            getParentPrint()->println(WiFi.gatewayIP().toString());
            getParentPrint()->print(F("Dns               : "));
            getParentPrint()->println(WiFi.dnsIP().toString());
        }
    }
}

void driver::update(int64_t /*delta*/) {
    if (updateStatus()) {
        updateLED();
        // do nothing more if status changed
        return;
    }
    // see if there is a new client or a client quit
    if (updateClientConnexion()) {
        return;
    }
    // listen to the telnet server
    listenTelnet();
}

bool driver::treatCommand(const core::command& cmd) {
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

void driver::printHelp() {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(F("Help on network interface"));
    getParentPrint()->println(F("netinfo      print information about network interface"));
    getParentPrint()->println(F("netstat      print information about network status"));
    getParentPrint()->println();
}

bool driver::updateStatus() {
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

void driver::updateServerState() {
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

bool driver::updateClientConnexion() {
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

void driver::updateLED() {
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

void driver::listenTelnet() {
    if (currentStatus != Status::HotspotClient && currentStatus != Status::ConnectedClient)
        return;

    if (client.available() > 0) {
        delay(10);
        core::command cmd(core::source::TELNET);
        while (client.available() > 0) {
            char c = static_cast<char>(client.read());
            if (c == '\n') break;
            if (c == '\r') break;
            if (!cmd.putChar(c)) break;
        }
        if (cmd.empty())
            return;
        if (getParent() != nullptr) {
            getParent()->pushCommand(cmd);
        }
    }
}

void driver::printStatus() {
    if (getParent() == nullptr)
        return;
    getParentPrint()->print(F("Network status:    "));
    switch (currentStatus) {
    case Status::Disabled:
        getParentPrint()->println(F("Disabled"));
        break;
    case Status::Connecting:
        getParentPrint()->println(F("Connecting"));
        break;
    case Status::Connected:
        getParentPrint()->println(F("Station waiting for a client"));
        break;
    case Status::ConnectedClient:
        getParentPrint()->println(F("Station with a client"));
        break;
    case Status::Hotspot:
        getParentPrint()->println(F("Hotspot waiting for client"));
        break;
    case Status::HotspotClient:
        getParentPrint()->println(F("hotspot with a client"));
        break;
    }
}

void driver::printWelcome() {
    client.println();
    client.println(F("Welcome on board!"));
    client.println(F("  _______         ______                      __ "));
    client.println(F(" |       |.-----.|   __ \\.-----.---.-.----.--|  |"));
    client.println(F(" |   -   ||     ||   __ <|  _  |  _  |   _|  _  |"));
    client.println(F(" |_______||__|__||______/|_____|___._|__| |_____|"));
    client.println(F("--------------------------------------------------"));
    client.print(F(" obd version: "));
    client.print(version);
    client.print(F(" Author: "));
    client.println(author);
    client.println(F("--------------------------------------------------"));
}

void driver::loadConfigFile() {
    filesystem::configFile file(getParent());
    file.loadConfig(getName());
    // parameters to load:
    if (file.hasKey("host")) {
        WiFi.hostname(file.getKey("host").c_str());
    }
}

void driver::saveConfigFile() const {
    filesystem::configFile file(getParent());
    // parameter to save
    file.addConfigParameter("host", WiFi.hostname());
    //
    file.saveConfig(getName());
}

}// namespace obd::network
