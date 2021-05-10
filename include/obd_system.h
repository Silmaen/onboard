//
// Created by damien.lachouette on 10/05/2021.
//

#pragma once
#include <Arduino.h>

namespace obd {

class system_impl {
public:
    void init();
    void update();
private:
    static void printSystemInfo(Stream& output);
};

extern system_impl system;

} // namespace obd
