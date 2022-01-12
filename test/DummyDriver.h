/**
 * @file DummyDriver.h
 * @author argawaen
 * @date 11/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <core/BaseDriver.h>

class DummyDriver: public obd::core::BaseDriver{
public:
    explicit DummyDriver(obd::core::System* parent) :
        BaseDriver(parent){};
    bool init() override  {_initialized = true; return _initialized;};
    void end() override { };
    void printInfo() override { };
    void update([[maybe_unused]] int64_t  delta) override { };
    void printHelp() override { };
    void loadConfigFile() override{ };
    void saveConfigFile() const override{ };
    [[nodiscard]] bool initialized()const{return _initialized;}
private:
    bool _initialized = false;
};
