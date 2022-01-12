/**
 * @file test_point.cpp
 * @author argawaen
 * @date 04/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"
#include "math/Point.h"

using namespace obd::math;

void test_point_operator_multiply() {
  Point a{10, 10};
  TEST_ASSERT_EQUAL((a * 4).x, (4 * a).x);
  TEST_ASSERT_EQUAL((a * 4).y, (4 * a).y);
  Point b{4, 4};
  TEST_ASSERT_EQUAL(40, (a * b).x);
  TEST_ASSERT_EQUAL(40, (a * b).y);
}

void test_point_operator_divide() {
  Point a{40, 40};
  TEST_ASSERT_EQUAL(10, (400 / a).x);
  TEST_ASSERT_EQUAL(10, (400 / a).y);
  TEST_ASSERT_EQUAL(10, (a / 4).x);
  TEST_ASSERT_EQUAL(10, (a / 4).y);
  TEST_ASSERT_EQUAL(0, (a / 0).x);
  TEST_ASSERT_EQUAL(0, (a / 0).y);
  Point b{4, 4};
  TEST_ASSERT_EQUAL(10, (a / b).x);
  TEST_ASSERT_EQUAL(10, (a / b).y);
  b = {4, 0};
  TEST_ASSERT_EQUAL(0, (a / b).x);
  TEST_ASSERT_EQUAL(0, (a / b).y);
  TEST_ASSERT_EQUAL(0, (400 / b).x);
  TEST_ASSERT_EQUAL(0, (400 / b).y);
}

void test_point_operator_add_sub() {
  Point a{40, 40};
  Point b{8, 14};
  TEST_ASSERT_EQUAL(48, (a + b).x);
  TEST_ASSERT_EQUAL(54, (a + b).y);
  TEST_ASSERT_EQUAL(32, (a - b).x);
  TEST_ASSERT_EQUAL(26, (a - b).y);
}

void test_point_operator_minmax() {
  Point a{85, 98};
  Point alow{150, 150};
  Point ahigh{50, 50};
  Point aa = clamp(a, ahigh, alow);
  TEST_ASSERT_EQUAL(a.x, aa.x);
  TEST_ASSERT_EQUAL(a.y, aa.y);
  aa = clamp(ahigh, a, alow);
  TEST_ASSERT_EQUAL(a.x, aa.x);
  TEST_ASSERT_EQUAL(a.y, aa.y);
  aa = clamp(alow, ahigh, a);
  TEST_ASSERT_EQUAL(a.x, aa.x);
  TEST_ASSERT_EQUAL(a.y, aa.y);

  aa = min(a, ahigh);
  TEST_ASSERT_EQUAL(ahigh.x, aa.x);
  TEST_ASSERT_EQUAL(ahigh.y, aa.y);
  aa = max(a, alow);
  TEST_ASSERT_EQUAL(alow.x, aa.x);
  TEST_ASSERT_EQUAL(alow.y, aa.y);
  aa = min(a, alow);
  TEST_ASSERT_EQUAL(a.x, aa.x);
  TEST_ASSERT_EQUAL(a.y, aa.y);
  aa = max(a, ahigh);
  TEST_ASSERT_EQUAL(a.x, aa.x);
  TEST_ASSERT_EQUAL(a.y, aa.y);
}

void test_all() {
  UNITY_BEGIN();
  RUN_TEST(test_point_operator_multiply);
  RUN_TEST(test_point_operator_divide);
  RUN_TEST(test_point_operator_add_sub);
  RUN_TEST(test_point_operator_minmax);
  UNITY_END();
}
