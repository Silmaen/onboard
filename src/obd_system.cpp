/**
 * @author Silmaen
 * @date 10/05/2021.
 */

#include <obd_filesystem.h>
#include <obd_network.h>
#include <obd_system.h>
#include <obd_usbserial.h>

namespace obd::core {

system::system() {
    drivers.push_back(new StatusLed(this));
    drivers.push_back(new UsbSerial(this));
    drivers.push_back(new filesystem::driver(this));
    drivers.push_back(new network::driver(this));
}

void system::init() {
    for (auto* driver : drivers) {
        driver->init();
        driver->printInfo();
    }
}

void system::update() {
    // update timestamp
    timestamp = millis();
    // update drivers
    for (auto* driver : drivers) {
        driver->update(timestamp);
    }
    // treat the command queue
    treatCommands();
}

void system::treatCommands() {
    while (!commands.empty()) {
        auto& cmd = commands.front();
        cmd.printCmd(outputs);
        bool treated = false;
        for (auto* driver : drivers) {
            treated = driver->treatCommand(cmd);
            if (treated)
                break;
        }
        if (treated) {
            commands.pop();
            continue;
        }
        if (cmd.isCmd(F("dmesg"))) {
            printSystemInfo();
        } else if (cmd.isCmd(F("help"))) {
            printHelp(cmd.getParams());
        } else {
            outputs.println("Unknown command");
        }
        commands.pop();
    }
}

void system::printKernelInfo() {
    // general info on chipset
    outputs.print(F("Chip Id:              0x"));
    outputs.println(EspClass::getChipId(), HEX);
    outputs.print(F("Core Version:         "));
    outputs.println(EspClass::getCoreVersion());
    outputs.print(F("Full Version:         "));
    outputs.println(EspClass::getFullVersion());
    outputs.print(F("CPU freq:             "));
    outputs.print(EspClass::getCpuFreqMHz());
    outputs.println(F("MHz"));

    // infos on the reset
    outputs.print(F("Reset Info:           "));
    outputs.println(EspClass::getResetInfo());
    outputs.print(F("boot version:         "));
    outputs.println(EspClass::getBootVersion());
    outputs.print(F("boot mode:            "));
    outputs.println(EspClass::getBootMode() == 0 ? F("SYS_BOOT_ENHANCE_MODE") : F("SYS_BOOT_NORMAL_MODE"));

    // HEAP memory statistics:
    uint32_t h_free = 0;
    uint16_t h_max  = 0;
    uint8_t h_frag  = 0;
    EspClass::getHeapStats(&h_free, &h_max, &h_frag);
    outputs.print(F("Heap:                 free: "));
    outputs.print(h_free);
    outputs.print(F(" / "));
    outputs.print(h_max);
    outputs.print(F(" Fragment: "));
    outputs.print(static_cast<int>(h_frag));
    outputs.print(F(" Free stack: "));
    outputs.println(EspClass::getFreeContStack());

    // flash memory info
    outputs.print(F("Flash Chip Id:        0x"));
    outputs.println(EspClass::getFlashChipId(), HEX);
    outputs.print(F("Flash Chip Vendor Id: 0x"));
    outputs.println(EspClass::getFlashChipVendorId(), HEX);

    //gets the actual chip size based on the flash id
    outputs.print(F("Flash Chip real size: "));
    outputs.println(EspClass::getFlashChipRealSize());
    //gets the size of the flash as set by the compiler
    outputs.print(F("Flash Chip size:      "));
    outputs.println(EspClass::getFlashChipSize());
    outputs.print(F("Flash Chip speed:     "));
    outputs.println(EspClass::getFlashChipSpeed());
    FlashMode_t a = EspClass::getFlashChipMode();
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
    uint32_t ss  = EspClass::getSketchSize();
    uint32_t fss = EspClass::getFreeSketchSpace();
    outputs.print(F("Sketch size:          "));
    outputs.print(ss);
    outputs.print(F(" / "));
    outputs.println(ss + fss);
}

void system::printSystemInfo() {
    printKernelInfo();
    for (auto* driver : drivers) {
        driver->printInfo();
    }
}

baseDriver* system::getDriver(const String& name) {
    for (auto* driver : drivers) {
        if (driver->getName() == name) {
            return driver;
        }
    }
    return nullptr;
}

void system::printHelp(const String& param) {
    outputs.println(F("SYSTEM HELP"));
    if (param.isEmpty()) {
        outputs.println(F("please give a subcategory for the specific help."));
        outputs.println(F("valid categories are:"));
        outputs.println(F("kernel"));
        for (auto* driver : drivers) {
            outputs.println(driver->getName());
        }
        return;
    }
    if (param == "kernel") {
        outputs.println(F("dmesg       print all system info"));
        outputs.println(F("help <sub>  get help on specific category"));
        return;
    }
    for (auto* driver : drivers) {
        if (param == driver->getName()) {
            driver->printHelp();
            return;
        }
    }
}


}// namespace obd::core
