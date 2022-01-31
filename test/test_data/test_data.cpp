/**
 * @file test_data.cpp
 * @author argawaen
 * @date 12/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"
#include "data/Series.h"

void test_series() {
    obd::data::Series<float, 10> data;
    TEST_ASSERT_EQUAL(0, data.getLength());
    TEST_ASSERT_EQUAL(0, data.getIndex());
    data.appendData(1);
    data.appendData(2);
    data.appendData(3);
    TEST_ASSERT_EQUAL_FLOAT(2, data.mean());
    data.appendData(4);
    data.appendData(5);
    data.appendData(6);
    data.appendData(7);
    data.appendData(8);
    data.appendData(9);
    data.appendData(10);
    data.appendData(11);             // this one should take the place of the '1'.
    TEST_ASSERT_EQUAL_FLOAT(2, data.min());// so the min should equals 2
    TEST_ASSERT_EQUAL_FLOAT(11, data.max());
    TEST_ASSERT_EQUAL_FLOAT(6.5, data.mean());
    TEST_ASSERT_EQUAL_FLOAT(50.5, data.meanSQ());
    TEST_ASSERT_EQUAL_FLOAT(2.872281, data.standardDeviation());
    TEST_ASSERT_EQUAL_FLOAT(8.25, data.variance());
}

void test_all() {
    UNITY_BEGIN();
    RUN_TEST(test_series);
    UNITY_END();
}
