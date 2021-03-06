/**
 * @file fakeArduino.h
 * @author argawaen
 * @date 05/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#ifdef ESP8266
#include <LittleFS.h>
#endif
#else
//#include "fakeSerial.h"
#include "fakeTiming.h"

/// Compatibility: normally put the const string into flash memory
#define F(x) x

/**
 * @brief Convert a double number into string
 * @param number The number to write
 * @param width The size in char number
 * @param prec The number of digits
 * @param s A char buffer
 * @return The char buffer
 */
char *dtostrf(double number, signed char width, unsigned char prec, char *s);

/**
 * @brief Function executed only one at the boot of the program
 */
void setup();
/**
 * @brief Program executed in an infinite loop.
 */
void loop();

#endif
