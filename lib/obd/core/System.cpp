/**
 * @file System.cpp
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "System.h"
#include "com/Console.h"
#include "gfx/StatusLed.h"
#include "fs/FileSystem.h"
#include "time/Clock.h"

namespace obd::core {

System::System() {
    // drivers that are always presents
    addDriver<gfx::StatusLed>();
    addDriver<com::Console>();
    addDriver<fs::FileSystem>();
    addDriver<time::Clock>();
}

void System::init() {
    initialized = true;
    for (const auto& driver : drivers) {
        driver->init();
        driver->printInfo();
    }
}

void System::update() {
    // update timestamp
    uint64_t timeStamp = micros();
    int64_t delta = static_cast<int64_t>(timeStamp) - static_cast<int64_t>(timestamp);
    timestamp     = timeStamp;
    timeData.appendData(delta);
    // update drivers
    for (const auto& driver : drivers) {
        driver->update(delta);
    }
    // treat the command queue
    treatCommands();
}

void System::printKernelInfo() {
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

void System::printSystemInfo() {
    printKernelInfo();
    for (const auto& driver : drivers) {
        driver->printInfo();
    }
}

void System::printHelp(const std::string& param) {
    outputs.println(F("SYSTEM HELP"));
    if (param.empty()) {
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

void System::printTimeStat() {
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


}// namespace obd::core
