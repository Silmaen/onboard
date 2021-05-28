//
// Created by damien.lachouette on 10/05/2021.
//

#include <obd_system.h>


#include <obd_filesystem.h>
#include <obd_network.h>

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
    outputs.addPrint(&Serial);
    Serial.println();
    Serial.println(F("SYSTEM INIT"));
    printKernelInfo();

    // LED
    pinMode(LED_BUILTIN, OUTPUT);
    ledSetState();

    // filesystem
    filesystem.init();
    filesystem.printInfo(Serial);

    // network
    network.init(&outputs);

}

void impl::update() {
    updateLedState();
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
    network.update(&commands);
    // treat the command queue
    treatCommands();

    // update timestamp
    timestamp = millis();
}

void impl::treatCommands() {
    while (!commands.empty()) {
        auto& cmd = commands.front();
        cmd.printCmd(outputs);
        if (cmd.isCmd("dmesg")) {
            printSystemInfo();
        }else if (cmd.isCmd("pwd")){
            filesystem.pwd(outputs);
        }else if (cmd.isCmd("ls")){
            filesystem.ls(outputs, cmd.getParams());
        }else if (cmd.isCmd("cd")){
            filesystem.cd(outputs, cmd.getParams());
        }else if (cmd.isCmd("mkdir")){
            filesystem.mkdir(outputs, cmd.getParams());
        }else if (cmd.isCmd("rm")){
            filesystem.rm(outputs, cmd.getParams());
        }else if (cmd.isCmd("netinfo")){
            printNetworkInfo();
        }else if (cmd.isCmd("led")){
            char buf[30];
            strcpy(buf,cmd.getParams());
            if (strcmp(buf, "off") == 0)
                ledSetState();
            else if (strcmp(buf, "solid") == 0)
                ledSetState(LedState::Solid);
            else if (strcmp(buf, "blink") == 0)
                ledSetState(LedState::Blink);
            else {
                outputs.println("Unknown led state");
            }
        }else{
            outputs.println("Unknown command");
        }
        commands.pop();
    }
}

void impl::printKernelInfo() {
    // general info on chipset
    outputs.print(F("Chip Id:              0x"));
    outputs.println(ESP.getChipId(), HEX);
    outputs.print(F("Core Version:         "));
    outputs.println(ESP.getCoreVersion());
    outputs.print(F("Full Version:         "));
    outputs.println(ESP.getFullVersion());
    outputs.print(F("CPU freq:             "));
    outputs.print(ESP.getCpuFreqMHz());
    outputs.println(F("MHz"));

    // infos on the reset
    outputs.print(F("Reset Info:           "));
    outputs.println(ESP.getResetInfo());
    outputs.print(F("boot version:         "));
    outputs.println(ESP.getBootVersion());
    outputs.print(F("boot mode:            "));
    outputs.println(ESP.getBootMode() == 0 ? F("SYS_BOOT_ENHANCE_MODE") : F("SYS_BOOT_NORMAL_MODE"));

    // HEAP memory statistics:
    uint32_t h_free = 0;
    uint16_t h_max = 0;
    uint8_t h_frag = 0;
    ESP.getHeapStats(&h_free, &h_max, &h_frag);
    outputs.print(F("Heap:                 free: "));
    outputs.print(h_free);
    outputs.print(F(" / "));
    outputs.print(h_max);
    outputs.print(F(" Fragment: "));
    outputs.print((int) h_frag);
    outputs.print(F(" Free stack: "));
    outputs.println(ESP.getFreeContStack());

    // flash memory info
    outputs.print(F("Flash Chip Id:        0x"));
    outputs.println(ESP.getFlashChipId(), HEX);
    outputs.print(F("Flash Chip Vendor Id: 0x"));
    outputs.println(ESP.getFlashChipVendorId(), HEX);

    //gets the actual chip size based on the flash id
    outputs.print(F("Flash Chip real size: "));
    outputs.println(ESP.getFlashChipRealSize());
    //gets the size of the flash as set by the compiler
    outputs.print(F("Flash Chip size:      "));
    outputs.println(ESP.getFlashChipSize());
    outputs.print(F("Flash Chip speed:     "));
    outputs.println(ESP.getFlashChipSpeed());
    FlashMode_t a = ESP.getFlashChipMode();
    outputs.print(F("Flash mode:           "));
    switch (a) {
        case FM_QIO:
            outputs.println(F("QIO"));
            break;
        case FM_QOUT:
            outputs.println(F("QOUT"));
            break;
        case FM_DIO:
            outputs.println(F("DIO"));
            break;
        case FM_DOUT:
            outputs.println(F("DOUT"));
            break;
        default:
            outputs.println(F("UNKNOWN"));
            break;
    }

    // sketch size
    uint32_t ss = ESP.getSketchSize();
    uint32_t fss = ESP.getFreeSketchSpace();
    outputs.print(F("Sketch size:          "));
    outputs.print(ss);
    outputs.print(" / ");
    outputs.println(ss + fss);
}

void impl::printSystemInfo(){
    printKernelInfo();
    filesystem.printInfo(outputs);
}

void impl::printNetworkInfo() {
    network.printInfo();
}

void impl::updateLedState() {
    switch(ledState){
        case LedState::Off:
        case LedState::Solid:
            break;
        case LedState::Blink:
            if ((timestamp - ledTime) > 1000) {
                ledVal = !ledVal;
                ledTime = timestamp;
            }
            digitalWrite(LED_BUILTIN,static_cast<uint8_t>(!ledVal));
            break;
    }

}

void impl::ledSetState(LedState state) {
    ledState = state;
    switch(ledState){
        case LedState::Off:
            ledVal = false;
            break;
        case LedState::Solid:
            ledVal = true;
            break;
        case LedState::Blink:
            ledVal = true;
            ledTime = timestamp;
            break;
    }
    digitalWrite(LED_BUILTIN,static_cast<uint8_t>(!ledVal));
}

}// namespace system
}// namespace obd
