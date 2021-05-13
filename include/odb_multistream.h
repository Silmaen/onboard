/**
 * \author argawaen 
 * \date 13/05/2021
 *
 */

#pragma once
#include <Print.h>
#include <vector>

namespace obd {
namespace system {

/**
 * @brief Simple override of the Print class to allow multiple output
 */
class MultiPrint : public Print {
public:
    void addPrint(Print* str){
        _prints.push_back(str);
    }
    size_t write(uint8_t a) override {
        for(auto* print: _prints)
            print->write(a);
        return 1;
    }
private:
    std::vector<Print*> _prints = {};
};

} // namespace system
} // namespace obd
