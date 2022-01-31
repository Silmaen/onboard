/**
 * @author Silmaen
 * @date 10/05/2021.
 */

#include <obd_configfile.h>
#include <obd_filesystem.h>
#include <obd_runcam.h>
#include <obd_system.h>
#include <obd_systemtime.h>
#include <obd_usbserial.h>
#include <obd_webserver.h>
#include <obd_wifidriver.h>
#include <user_interface.h>

namespace obd::core {

System::System() {
    drivers.addDriver<core::StatusLed>(this);
    drivers.addDriver<network::UsbSerial>(this);
    drivers.addDriver<file::FileSystem>(this);
    drivers.addDriver<network::WifiDriver>(this);
    drivers.addDriver<time::Clock>(this);
    drivers.addDriver<network::WebServer>(this);
    drivers.addDriver<video::RunCam>(this);
}

void System::init() {
    for (const auto& driver : drivers) {
        driver->init();
        driver->printInfo();
    }
}

void System::update() {
    // update timestamp
    uint64_t ts    = micros();
    int64_t delta = static_cast<int64_t>(ts) - static_cast<int64_t>(timestamp);
    timestamp     = ts;
    timeData.appendData(delta);
    // update drivers
    for (const auto& driver : drivers) {
        driver->update(delta);
    }
    // treat the command queue
    treatCommands();
}

void System::treatCommands() {
    // only one command is treated: if the queue contains more, the command will be treated at the next loop
    if (!commands.empty()) {
        auto& cmd = commands.front();
        cmd.printCmd(outputs);
        bool treated = false;
        for (const auto& driver : drivers) {
            treated = driver->treatCommand(cmd);
            if (treated)
                break;
        }
        if (treated) {
            commands.pop();
            return;
        }
        if (cmd.isCmd(F("dmesg"))) {
            printSystemInfo();
        } else if (cmd.isCmd(F("help"))) {
            printHelp(cmd.getParams());
        } else if (cmd.isCmd(F("timeStat"))) {
            printTimeStat();
        } else if (cmd.isCmd(F("cfgSave"))) {
            saveAllConfig();
        } else if (cmd.isCmd(F("cfgLoad"))) {
            loadAllConfig();
        } else {
            outputs.println(F("Unknown command"));
        }
        commands.pop();
    }
}

void System::printKernelInfo() {
    // general info on chipset
    outputs.println(F(" ----- KERNEL INFORMATION -----"));
    outputs.print(F("Chip Id           : 0x"));
    outputs.println(EspClass::getChipId(), HEX);
    outputs.print(F("Core Version      : "));
    outputs.println(EspClass::getCoreVersion());
    outputs.print(F("Full Version      : "));
    outputs.println(EspClass::getFullVersion());
    outputs.print(F("CPU freq          : "));
    outputs.print(EspClass::getCpuFreqMHz());
    outputs.println(F("MHz"));

    // infos on the reset
    outputs.print(F("Reset Info        : "));
    outputs.println(EspClass::getResetInfo());
    outputs.print(F("boot version      : "));
    outputs.print(EspClass::getBootVersion());
    outputs.print(F(" mode: "));
    outputs.println(EspClass::getBootMode() == 0 ? F("SYS_BOOT_ENHANCE_MODE") : F("SYS_BOOT_NORMAL_MODE"));

    // HEAP memory statistics:
    uint32_t h_free = 0;
    uint16_t h_max  = 0;
    uint8_t h_frag  = 0;
    EspClass::getHeapStats(&h_free, &h_max, &h_frag);
    outputs.print(F("Heap              : free: "));
    outputs.print(h_free);
    outputs.print(F(" / "));
    outputs.print(h_max);
    outputs.print(F(" Fragment: "));
    outputs.print(static_cast<int>(h_frag));
    outputs.print(F(" Free stack: "));
    outputs.println(EspClass::getFreeContStack());

    // flash memory info
    outputs.print(F("Flash Chip Id     : 0x"));
    outputs.print(EspClass::getFlashChipId(), HEX);
    outputs.print(F(" Vendor Id: 0x"));
    outputs.println(EspClass::getFlashChipVendorId(), HEX);

    //gets the actual chip size based on the flash id should be equal to
    //gets the size of the flash as set by the compiler
    outputs.print(F("Flash Chip size   : "));
    outputs.print(EspClass::getFlashChipRealSize());
    outputs.print(F(" == "));
    outputs.println(EspClass::getFlashChipSize());

    outputs.print(F("Flash Chip speed  : "));
    outputs.print(EspClass::getFlashChipSpeed());
    FlashMode_t a = EspClass::getFlashChipMode();
    outputs.print(F("  mode: "));
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
    outputs.print(F("Sketch size       : "));
    outputs.print(ss);
    outputs.print(F(" / "));
    outputs.println(ss + fss);
}

void System::printSystemInfo() {
    printKernelInfo();
    for (const auto& driver : drivers) {
        driver->printInfo();
    }
}

void System::printHelp(const String& param) {
    outputs.println(F("SYSTEM HELP"));
    if (param.isEmpty()) {
        outputs.println(F("please give a subcategory for the specific help."));
        outputs.println(F("valid categories are:"));
        outputs.println(F(" - kernel"));
        for (const auto& driver : drivers) {
            outputs.print(F(" - "));
            outputs.println(driver->getName());
        }
        return;
    }
    if (param == "kernel") {
        outputs.println(F("dmesg          print all System info"));
        outputs.println(F("help    <sub>  get help on specific category"));
        outputs.println(F("cfgSave        save configuration to files"));
        outputs.println(F("cfgLoad        load configuration from files"));
        return;
    }
    auto res = std::find_if(drivers.begin(), drivers.end(),
                            [param](const std::shared_ptr<obd::core::BaseDriver>& b) { return b->getName() == param; });
    if (res != drivers.end()) {
        (*res)->printHelp();
        return;
    }
    outputs.println(F("invalid category given."));
    outputs.println(F("valid categories are:"));
    outputs.println(F(" - kernel"));
    for (const auto& driver : drivers) {
        outputs.print(F(" - "));
        outputs.println(driver->getName());
    }
}

void System::loadAllConfig() {
    // load for the System
    // load for the drivers
    for (const auto& driver : drivers) {
        driver->loadConfigFile();
    }
}

void System::saveAllConfig() {
    // save for the System
    // save for the drivers
    for (const auto& driver : drivers) {
        driver->saveConfigFile();
    }
}

void System::printTimeStat() {
    outputs.println(F(" -=| timing statistics |=- "));

    outputs.print(F("L / I: "));
    outputs.print(timeData.getLength());
    outputs.print(F(" / "));
    outputs.println(timeData.getIndex());
    float fps = 1000000.0F / static_cast<float>(timeData.mean());
    outputs.print(F("Average fps: "));
    outputs.println(fps);
    outputs.print(F("Average timedelta / variance: "));
    outputs.print(timeData.mean());
    outputs.print(F(" / "));
    outputs.println(timeData.variance());
    outputs.print(F("AverageSQ timedelta / standardDeviation: "));
    outputs.print(timeData.meanSQ());
    outputs.print(F(" / "));
    outputs.println(timeData.standardDeviation());
    outputs.print(F("min / max: "));
    outputs.print(timeData.min());
    outputs.print(F(" / "));
    outputs.println(timeData.max());
}

}// namespace obd::core
