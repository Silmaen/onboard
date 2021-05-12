//
// Created by damien.lachouette on 10/05/2021.
//

#include <obd_system.h>
#ifdef ESP8266
#include <Esp.h>
#include <ArduinoJson.h>
#endif

#include <obd_filesystem.h>

namespace obd {

system::impl hardware;

namespace system {

void impl::init() {
    // -----------------------------------------------------------------------
    // initialize the USB Serial for debug
    // -----------------------------------------------------------------------
    // Reuse default Serial port rate, so the bootloader
    // messages are also readable.
    Serial.begin(115200);
    uint32_t br = Serial.detectBaudrate(1000);
    if (br != 0U) {
        Serial.updateBaudRate(br);
    }
    currStreams.push_back(&Serial);
    Serial.println();
    Serial.println(F("SYSTEM INIT"));
    printKernelInfo(&Serial);
    filesystem.init();
    filesystem.printInfo(&Serial);
}

void impl::update() {
    // read on serial
    if (Serial.available() > 0){
        delay(10);
        command cmd(source::USB);
        while(Serial.available()>0) {
            char c = Serial.read();
            if (c == '\n') break;
            if (!cmd.putChar(c)) break;
        }
        commands.push(cmd);
    }
    // treat the command queue
    treatCommands();
}

void impl::treatCommands() {
    while (!commands.empty()) {
        auto& cmd = commands.front();
        cmd.printCmd(Serial);
        if (cmd.isCmd("dmesg")) {
            for (auto *stream : currStreams)
                printSystemInfo(stream);
        }else if (cmd.isCmd("dmesg")){

        }else{
            for(auto *stream: currStreams)
                stream->println("Unknown command");
        }
        commands.pop();
    }
}

void impl::printKernelInfo(Stream *output) {
    // general info on chipset
    output->print(F("Chip Id:              0x"));
    output->println(ESP.getChipId(), HEX);
    output->print(F("Core Version:         "));
    output->println(ESP.getCoreVersion());
    output->print(F("Full Version:         "));
    output->println(ESP.getFullVersion());
    output->print(F("CPU freq:             "));
    output->print(ESP.getCpuFreqMHz());
    output->println(F("MHz"));

    // infos on the reset
    output->print(F("Reset Info:           "));
    output->println(ESP.getResetInfo());
    output->print(F("boot version:         "));
    output->println(ESP.getBootVersion());
    output->print(F("boot mode:            "));
    output->println(ESP.getBootMode() == 0 ? F("SYS_BOOT_ENHANCE_MODE") : F("SYS_BOOT_NORMAL_MODE"));

    // HEAP memory statistics:
    uint32_t h_free = 0;
    uint16_t h_max = 0;
    uint8_t h_frag = 0;
    ESP.getHeapStats(&h_free, &h_max, &h_frag);
    output->print(F("Heap:                 free: "));
    output->print(h_free);
    output->print(F(" / "));
    output->print(h_max);
    output->print(F(" Fragment: "));
    output->print((int) h_frag);
    output->print(F(" Free stack: "));
    output->println(ESP.getFreeContStack());

    // flash memory info
    output->print(F("Flash Chip Id:        0x"));
    output->println(ESP.getFlashChipId(), HEX);
    output->print(F("Flash Chip Vendor Id: 0x"));
    output->println(ESP.getFlashChipVendorId(), HEX);

    //gets the actual chip size based on the flash id
    output->print(F("Flash Chip real size: "));
    output->println(ESP.getFlashChipRealSize());
    //gets the size of the flash as set by the compiler
    output->print(F("Flash Chip size:      "));
    output->println(ESP.getFlashChipSize());
    output->print(F("Flash Chip speed:     "));
    output->println(ESP.getFlashChipSpeed());
    FlashMode_t a = ESP.getFlashChipMode();
    output->print(F("Flash mode:           "));
    switch (a) {
        case FM_QIO:
            output->println(F("QIO"));
            break;
        case FM_QOUT:
            output->println(F("QOUT"));
            break;
        case FM_DIO:
            output->println(F("DIO"));
            break;
        case FM_DOUT:
            output->println(F("DOUT"));
            break;
        default:
            output->println(F("UNKNOWN"));
            break;
    }

    // sketch size
    uint32_t ss = ESP.getSketchSize();
    uint32_t fss = ESP.getFreeSketchSpace();
    output->print(F("Sketch size:          "));
    output->print(ss);
    output->print(" / ");
    output->println(ss + fss);
}

void impl::printSystemInfo(Stream *output){
    printKernelInfo(output);
    filesystem.printInfo(output);
}

bool impl::loadConfig() {
    File cfg = filesystem.open("/config.dat","r");
    if (!cfg) {
        for(auto* stream:currStreams)
            stream->println(F("loadConfig: unable to open configuration File"));
        return false;
    }
    if (cfg.size() > maxConfigFileSize) {
        for(auto* stream:currStreams)
            stream->println(F("loadConfig: file too large"));
        return false;
    }
    std::unique_ptr<char[]> buf(new char[cfg.size()]);
    cfg.readBytes(buf.get(),cfg.size());
    StaticJsonDocument<200> doc;
    auto error = deserializeJson(doc, buf.get());
    if (error) {
        for(auto* stream:currStreams)
            stream->println(F("loadConfig: failed to parse config file"));
        return false;
    }
    return true;
}
bool impl::saveConfig() {
    return true;
}


}// namespace system
}// namespace obd
