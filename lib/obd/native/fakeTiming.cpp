/**
 * @file fakeTiming.cpp
 * @author argawaen
 * @date 06/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "fakeTiming.h"
#include <chrono>

/// Clock used in the next routines
using internal_clock = std::chrono::high_resolution_clock;

/// Alias for the time point
using time_point = internal_clock::time_point;

/// Alias for milliseconds
using milliseconds = std::chrono::milliseconds;

/// Alias for microseconds
using microseconds = std::chrono::microseconds;

/// Save of the program start date
static time_point startingPoint;

void initTimer() { startingPoint = internal_clock::now(); }

unsigned long millis() {
  return std::chrono::duration_cast<milliseconds>(internal_clock::now() -
                                                  startingPoint)
      .count();
}

unsigned long micros() { return micros64(); }

uint64_t micros64() {
  return std::chrono::duration_cast<microseconds>(internal_clock::now() -
                                                  startingPoint)
      .count();
}

void delay(unsigned long ms) {
  time_point start = internal_clock::now();
  while (std::chrono::duration_cast<milliseconds>(internal_clock::now() - start)
             .count() < ms)
    ;
}

void delayMicroseconds(unsigned int us) {
  time_point start = internal_clock::now();
  while (std::chrono::duration_cast<microseconds>(internal_clock::now() - start)
             .count() < us)
    ;
}
