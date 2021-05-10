//
// Created by damien.lachouette on 10/05/2021.
//

#include <obd_system.h>
#ifdef ESP8266
#include <Esp.h>
#endif


namespace obd {

system_impl system;



void obd::system_impl::init() {
    // -----------------------------------------------------------------------
    // initialize the USB Serial for debug
    // -----------------------------------------------------------------------
    // Reuse default Serial port rate, so the bootloader
    // messages are also readable.


    Serial.begin(74880);
    uint32_t br = Serial.detectBaudrate(1000);
    if (br != 0U) {
        Serial.updateBaudRate(br);
    }
    Serial.println();
    Serial.println(F("SYSTEM INIT"));
    printSystemInfo(Serial);
}

void system_impl::update() {

}


void system_impl::printSystemInfo(Stream &output) {
    // general info on chipset
    output.print(F("Chip Id: 0x"));
    output.println(ESP.getChipId(), HEX);
    output.print(F("Core Version: "));
    output.println(ESP.getCoreVersion());
    output.print(F("Full Version: "));
    output.println(ESP.getFullVersion());
    output.print(F("CPU freq: "));
    output.print(ESP.getCpuFreqMHz());
    output.println(F("MHz"));

    // infos on the reset
    output.print(F("getResetInfo(): "));
    output.println(ESP.getResetInfo());
    output.print(F("boot version: "));
    output.println(ESP.getBootVersion());
    output.print(F("boot mode: "));
    output.println(ESP.getBootMode() == 0 ? F("SYS_BOOT_ENHANCE_MODE") : F("SYS_BOOT_NORMAL_MODE"));


    // HEAP memory statistics:
    uint32_t h_free=0;
    uint16_t h_max= 0;
    uint8_t h_frag=0;
    ESP.getHeapStats(&h_free, &h_max, &h_frag);
    output.print(F("Heap: free: "));
    output.print(h_free);
    output.print(F(" / "));
    output.print(h_max);
    output.print(F(" Fragment: "));
    output.print((int)h_frag);
    output.print(F(" Free stack: "));
    output.println(ESP.getFreeContStack());

    // flash memory info
    output.print(F("Flash Chip Id: 0x"));
    output.println(ESP.getFlashChipId(),HEX);
    output.print(F("Flash Chip Vendor Id: 0x"));
    output.println(ESP.getFlashChipVendorId(),HEX);

    //gets the actual chip size based on the flash id
    output.print(F("Flash Chip real size: "));
    output.println(ESP.getFlashChipRealSize());
    //gets the size of the flash as set by the compiler
    output.print(F("Flash Chip size: "));
    output.println(ESP.getFlashChipSize());
    output.print(F("Flash Chip speed: "));
    output.println(ESP.getFlashChipSpeed());
    FlashMode_t a = ESP.getFlashChipMode();
    switch(a){
        case FM_QIO:
            output.println(F("Flash mode QIO"));
            break;
        case FM_QOUT:
            output.println(F("Flash mode QOUT"));
            break;
        case FM_DIO:
            output.println(F("Flash mode DIO"));
            break;
        case FM_DOUT:
            output.println(F("Flash mode DOUT"));
            break;
        default:
            output.println(F("Flash mode UNKNOWN"));
            break;
    }
    output.print(F("Flash Chip size by chip ID: "));
    output.println(ESP.getFlashChipSizeByChipId());

    // sketch size
    uint32_t ss = ESP.getSketchSize();
    uint32_t fss = ESP.getFreeSketchSpace();
    output.print(F("Sketch size: "));
    output.print(ss);
    output.print(" / ");
    output.println(ss+fss);
}

}// namespace obd
