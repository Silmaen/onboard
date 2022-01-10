/**
 * @file test_color.cpp
 * @author argawaen
 * @date 04/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"
#include "gfx/Color.h"

using namespace obd::gfx;

void test_rgb565() {
  TEST_ASSERT_EQUAL(0b1111100000000000, red.toRGB565());
  TEST_ASSERT_EQUAL(0b1111111111100000, yellow.toRGB565());
  TEST_ASSERT_EQUAL(0b0000011111100000, green.toRGB565());
  TEST_ASSERT_EQUAL(0b0000011111111111, cyan.toRGB565());
  TEST_ASSERT_EQUAL(0b0000000000011111, blue.toRGB565());
  TEST_ASSERT_EQUAL(0b1111100000011111, magenta.toRGB565());
  TEST_ASSERT_EQUAL(0b0000000000000000, black.toRGB565());
  TEST_ASSERT_EQUAL(0b1111111111111111, white.toRGB565());
  TEST_ASSERT_EQUAL(0b0011100111100111, lightGrey.toRGB565());
  TEST_ASSERT_EQUAL(0b0111101111101111, grey.toRGB565());
  TEST_ASSERT_EQUAL(0b1011110111110111, darkGrey.toRGB565());
};

void test_rgb332() {
  TEST_ASSERT_EQUAL(0b11100000, red.toRGB332());
  TEST_ASSERT_EQUAL(0b11111100, yellow.toRGB332());
  TEST_ASSERT_EQUAL(0b00011100, green.toRGB332());
  TEST_ASSERT_EQUAL(0b00011111, cyan.toRGB332());
  TEST_ASSERT_EQUAL(0b00000011, blue.toRGB332());
  TEST_ASSERT_EQUAL(0b11100011, magenta.toRGB332());
  TEST_ASSERT_EQUAL(0b00000000, black.toRGB332());
  TEST_ASSERT_EQUAL(0b11111111, white.toRGB332());
  TEST_ASSERT_EQUAL(0b00100100, lightGrey.toRGB332());
  TEST_ASSERT_EQUAL(0b01101101, grey.toRGB332());
  TEST_ASSERT_EQUAL(0b10110110, darkGrey.toRGB332());
};

void test_all() {
  UNITY_BEGIN();
  RUN_TEST(test_rgb565);
  RUN_TEST(test_rgb332);
  UNITY_END();
}