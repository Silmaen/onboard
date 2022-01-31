/**
 * @file ConsoleOutput.h
 * @author argawaen
 * @date 13/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Output.h"

namespace obd::com {

/**
 * @brief Class handling output in the stdout
 */
class ConsoleOutput : public Output {
public:
    ConsoleOutput(const ConsoleOutput&) = delete;
    ConsoleOutput(ConsoleOutput&&)      = delete;
    ConsoleOutput& operator=(const ConsoleOutput&) = delete;
    ConsoleOutput& operator=(ConsoleOutput&&) = delete;

    /**
     * @brief Default Constructor
     */
    ConsoleOutput() = default;

    /// Destructor
    ~ConsoleOutput() override = default;

private:
    /**
    * @brief Print a string
    * @param data The String to print
    */
    void writeBytes(const OString& data) override;
};

}// namespace obd::com
