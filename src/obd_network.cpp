/**
 * \author argawaen 
 * \date 26/05/2021
 *
 */
#include <config.h>
#include <obd_network.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <Esp.h>
#endif
#include <obd_system.h>

namespace obd {

namespace network {

driver::driver(core::system *p) : baseDriver(p) {
    if (p != nullptr) {
        statusLed = p->getDriverAs<core::StatusLed>("Status Led");
    }
}

void driver::attachParent(core::system *p) {
    baseDriver::attachParent(p);
    if (p != nullptr) {
        statusLed = p->getDriverAs<core::StatusLed>("Status Led");
    }
}

void driver::init() {
    if (statusLed != nullptr)
        statusLed->setState(core::LedState::FasterBlink);
    WiFi.hostname(defaultHostname);
    WiFi.begin();
    if (getParentPrint() != nullptr)
        WiFi.printDiag(*getParentPrint());
}

void driver::printInfo() {
    if (getParentPrint() == nullptr)
        return;
    const char *const modes[] = {"Off", "Station", "Access Point", "Both"};
    getParentPrint()->print(F("Operation Mode      : "));
    getParentPrint()->println(modes[wifi_get_opmode()]);
    const char *const phymodes[] = {"", "b", "g", "n"};
    getParentPrint()->print(F("PHY mode            : 802.11"));
    getParentPrint()->println(phymodes[static_cast<int>(wifi_get_phy_mode())]);

    if ((wifi_get_opmode() == 2) || (wifi_get_opmode() == 3)) {
        // access point Infos
        getParentPrint()->println(F("Access point informations"));
    }
    if ((wifi_get_opmode() == 1) || (wifi_get_opmode() == 3)) {
        // station Infos
        getParentPrint()->println(F("Station informations"));
        getParentPrint()->print(F("Access point id     : "));
        getParentPrint()->println(wifi_station_get_current_ap_id());
        getParentPrint()->print(F("Access point SSID   : "));
        getParentPrint()->println(WiFi.SSID());
        getParentPrint()->print(F("Channel             : "));
        getParentPrint()->println(WiFi.channel());
        getParentPrint()->print(F("Connexion Status    : "));
        const char *const connStatus[] = {"idle", "connecting", "Wrong Password", "No AP found", "Connect fail", "got IP"};
        getParentPrint()->println(connStatus[static_cast<int>(wifi_station_get_connect_status())]);

        getParentPrint()->print(F("MAC address         : "));
        getParentPrint()->println(WiFi.macAddress());

        getParentPrint()->print("hostname                         : ");
        getParentPrint()->println(WiFi.hostname());
        if (WiFi.status() == WL_CONNECTED) {
            getParentPrint()->print("IP address          : ");
            getParentPrint()->println(WiFi.localIP().toString());
            getParentPrint()->print("Net Mask            : ");
            getParentPrint()->println(WiFi.subnetMask().toString());
            getParentPrint()->print("Gateway             : ");
            getParentPrint()->println(WiFi.gatewayIP().toString());
            getParentPrint()->print("Dns                 : ");
            getParentPrint()->println(WiFi.dnsIP().toString());
        }
    }
}

void driver::update(uint64_t timestamp) {
    if (updateStatus())
        updateLED();
}

bool driver::treatCommand(const core::command &cmd) {
    if (cmd.isCmd("netinfo")) {
        printInfo();
        return true;
    }
    return false;
}

void driver::printHelp() {
    if (getParentPrint() == nullptr)
        return;
    getParentPrint()->println(F("Help on network interface"));
    getParentPrint()->println(F("netinfo      print information about network interface"));
    getParentPrint()->println();
}
bool driver::updateStatus() {
    Status cal = Status::Connecting;
    if (WiFi.status() == WL_IDLE_STATUS) {
        cal = Status::Disabled;
    } else if (WiFi.status() == WL_CONNECTED) {
        if (WiFi.getMode() == WiFiMode::WIFI_AP) {
            cal = Status::Hotspot;
        } else {
            cal = Status::Connected;
        }
    }
    if (cal != currentStatus){
        currentStatus = cal;
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

}// namespace network
}// namespace obd
