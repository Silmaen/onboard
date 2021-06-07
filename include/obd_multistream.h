/**
 * @author Silmaen
 * @date 13/05/2021
 */

#pragma once
#include <Print.h>
#include <algorithm>
#include <vector>

namespace obd::core {

/**
 * @brief Simple override of the Print class to allow multiple output
 */
class MultiPrint : public Print {
public:
    /**
     * @brief add a print to the list
     * @param str the stream to add
     */
    void addPrint(Print* str) {
        _prints.push_back(str);
    }

    /**
     * @brief remove a print from the list
     * @param str the stream to remove
     */
    void removePrint(Print* str) {
        auto it = std::find(_prints.begin(), _prints.end(), str);
        if (it != _prints.end()) {
            _prints.erase(it);
        }
    }

    /**
     * @brief implementation of the write command
     * @param a byte to write
     * @return number of bytes written
     */
    size_t write(uint8_t a) override {
        for (auto* print : _prints)
            print->write(a);
        return 1;
    }

private:
    /// list of the print implementations
    std::vector<Print*> _prints = {};
};

}// namespace obd::core
