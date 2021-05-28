/**
 * \author argawaen 
 * \date 26/05/2021
 *
 */
#include <config.h>
#include <obd_network.h>
#ifdef ESP8266
#include <Esp.h>
#include <ESP8266WiFi.h>
#endif

namespace obd {

internal::network_impl network;

namespace internal {

void network_impl::init(Print* out) {
    output = out;
    WiFi.hostname(defaultHostname);
    WiFi.begin();
    if (output != nullptr)
        WiFi.printDiag(*output);
}

void network_impl::printInfo() {
    if (output == nullptr)
        return;
    const char* const modes[] = { "Off", "Station", "Access Point", "Both" };
    output->print(F("Operation Mode      : "));
    output->println(modes[wifi_get_opmode()]);
    const char* const phymodes[] = { "", "b", "g", "n" };
    output->print(F("PHY mode            : 802.11"));
    output->println(phymodes[static_cast<int>(wifi_get_phy_mode())]);

    if ((wifi_get_opmode() == 2) || (wifi_get_opmode() == 3)) {
        // access point Infos
        output->println(F("Access point informations"));
    }
    if ((wifi_get_opmode() == 1) || (wifi_get_opmode() == 3)) {
        // station Infos
        output->println(F("Station informations"));
        output->print(F("Access point id     : "));
        output->println(wifi_station_get_current_ap_id());
        output->print(F("Access point SSID   : "));
        output->println(WiFi.SSID());
        output->print(F("Channel             : "));
        output->println(WiFi.channel());
        output->print(F("Connexion Status    : "));
        const char* const connStatus[] = { "idle", "connecting", "Wrong Password", "No AP found" , "Connect fail", "got IP"};
        output->println(connStatus[static_cast<int>(wifi_station_get_connect_status())]);

        output->print(F("MAC address         : "));
        output->println(WiFi.macAddress());

        output->print("hostname                         : ");
        output->println(WiFi.hostname());
        if (WiFi.status() == WL_CONNECTED) {
            output->print("IP address          : ");
            output->println(WiFi.localIP().toString());
            output->print("Net Mask            : ");
            output->println(WiFi.subnetMask().toString());
            output->print("Gateway             : ");
            output->println(WiFi.gatewayIP().toString());
            output->print("Dns                 : ");
            output->println(WiFi.dnsIP().toString());
        }
    }
}
void network_impl::update(std::queue<system::command>* cmds) {

}

} // namespace internal
} // namespace obd
