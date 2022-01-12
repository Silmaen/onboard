/**
 * @file test_random.cpp
 * @author argawaen
 * @date 04/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"
#include "math/Random.h"

using namespace obd::math;

void test_rand() {
  Random rng;
  rng.setSeed(123456); // ensure all test equal
  TEST_ASSERT_EQUAL_UINT64(132366047211908ULL, rng.rand());
  TEST_ASSERT_EQUAL_UINT64(8689640596666966631ULL, rng.rand());
  TEST_ASSERT_EQUAL_UINT64(455850758477959267ULL, rng.rand());
  TEST_ASSERT_EQUAL_UINT64(13229926639038037763ULL, rng.rand());
};

void test_rand64() {
  Random rng{123456}; // ensure all test equal
  TEST_ASSERT_EQUAL_UINT64(2047211908UL, rng.rand64(4000000000ULL));
  TEST_ASSERT_EQUAL_UINT64(666966631ULL, rng.rand64(4000000000ULL));
  TEST_ASSERT_EQUAL_UINT64(2477959267ULL, rng.rand64(4000000000ULL));
  TEST_ASSERT_EQUAL_UINT64(3038037763ULL, rng.rand64(4000000000ULL));
  TEST_ASSERT_EQUAL_UINT64(2344641167ULL,
                           rng.rand64(2000000000ULL, 4000000000ULL));
};

void test_rand32() {
  Random rng;
  rng.setSeed(123456); // ensure all test equal
  TEST_ASSERT_EQUAL_UINT32(46814534UL, rng.rand32(48523698UL));
  TEST_ASSERT_EQUAL_UINT32(3293579UL, rng.rand32(48523698UL));
  TEST_ASSERT_EQUAL_UINT32(21138637UL, rng.rand32(48523698UL));
  TEST_ASSERT_EQUAL_UINT32(17580779UL, rng.rand32(48523698UL));
  TEST_ASSERT_EQUAL_UINT32(46727305UL, rng.rand32(28523698UL, 48523698UL));
};

void test_rand16() {
  Random rng;
  rng.setSeed(123456); // ensure all test equal
  TEST_ASSERT_EQUAL_UINT16(17348U, rng.rand16(22560U));
  TEST_ASSERT_EQUAL_UINT16(5351U, rng.rand16(22560U));
  TEST_ASSERT_EQUAL_UINT16(11587U, rng.rand16(22560U));
  TEST_ASSERT_EQUAL_UINT16(3203U, rng.rand16(22560U));
  TEST_ASSERT_EQUAL_UINT16(21859U, rng.rand16(12560U, 22560U));
};

void test_rand8() {
  Random rng;
  rng.setSeed(123456); // ensure all test equal
  TEST_ASSERT_EQUAL_UINT8(98U, rng.rand8(142U));
  TEST_ASSERT_EQUAL_UINT8(51U, rng.rand8(142U));
  TEST_ASSERT_EQUAL_UINT8(21U, rng.rand8(142U));
  TEST_ASSERT_EQUAL_UINT8(63U, rng.rand8(142U));
  TEST_ASSERT_EQUAL_UINT8(98U, rng.rand8(42U, 142U));
};

void test_all() {
  UNITY_BEGIN();
  RUN_TEST(test_rand);
  RUN_TEST(test_rand64);
  RUN_TEST(test_rand32);
  RUN_TEST(test_rand16);
  RUN_TEST(test_rand8);
  UNITY_END();
}
