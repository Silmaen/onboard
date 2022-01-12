/**
 * @file test_arduino.cpp
 * @author argawaen
 * @date 09/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "../test_base.h"

#include <iostream>

void test_delay() {
  auto startMilli = millis();
  auto startMicros = micros();
  auto startMicros64 = micros64();
  delay(1);
  delayMicroseconds(1);
  startMilli = millis() - startMilli;
  startMicros = micros() - startMicros;
  startMicros64 = micros64() - startMicros64;
  TEST_ASSERT_UINT32_WITHIN(1, 1, startMilli);
  TEST_ASSERT_UINT32_WITHIN(10, 1001, startMicros);
  TEST_ASSERT_UINT64_WITHIN(10, 1001, startMicros64);
}

void test_all() {
  UNITY_BEGIN();
  RUN_TEST(test_delay);
  UNITY_END();
}