/**
 * @file MultiOutput.h
 * @author argawaen
 * @date 10/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Output.h"
#include <algorithm>
#include <vector>

namespace obd::com {

/**
 * @brief Override output to allow multiple output
 */
class MultiOutput : public Output {
public:
    /**
     * @brief Add a print to the list
     * @param str The stream to add
     */
    void addOutput(Output* str) {
        _prints.push_back(str);
    }

    /**
     * @brief Remove a print from the list
     * @param str The stream to remove
     */
    void removeOutput(Output* str) {
        auto iter = std::find(_prints.begin(), _prints.end(), str);
        if (iter != _prints.end()) {
            _prints.erase(iter);
        }
    }

    /**
     * @brief Implementation of the write command
     * @param data Byte to write
     * @return Amount of bytes written
     */
    size_t write(uint8_t data) override {
        for (auto* print : _prints)
            print->write(data);
        return 1;
    }

private:
    /// List of the print implementations
    std::vector<Output*> _prints = {};
};

}// namespace obd::com
