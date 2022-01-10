/**
 * @file test_fileread.cpp
 * @author argawaen
 * @date 04/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"
#include <fstream>

void test_read() {
  std::ifstream file("data/title.odb", std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    TEST_FAIL_MESSAGE("Unable to find the file");
  }
  uint16_t w, h;
  file.read((char *)&w, 2);
  file.read((char *)&h, 2);
  TEST_ASSERT_EQUAL(800, w);
  TEST_ASSERT_EQUAL(480, h);
}

void test_all() {
  UNITY_BEGIN();
  RUN_TEST(test_read);
  UNITY_END();
}