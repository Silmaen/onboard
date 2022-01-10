/**
 * @file fakeArduino.cpp
 * @author argawaen
 * @date 05/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "fakeArduino.h"
#ifndef ARDUINO
/**
 * @brief Main entry point of the program simulating an Arduino one.
 * @return error code
 */
int main() {
  initTimer();
  setup();
#ifdef UNIT_TEST
  loop();
#else
  while (true) {
    loop();
  }
#endif
}

#endif
