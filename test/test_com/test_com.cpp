/**
 * @file test_com.cpp
 * @author argawaen
 * @date 12/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include <iostream>
#include <sstream>
#include "../test_base.h"

void test_output() {
#ifdef ARDUINO
    TEST_PASS()
#else
    obd::com::Output out;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ostringstream strOut;
    std::cout.rdbuf(strOut.rdbuf());
    out.println(static_cast<int8_t>('G'));
    TEST_ASSERT_EQUAL_STRING("G\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<int8_t>('G'), obd::com::Format::Decimal);
    TEST_ASSERT_EQUAL_STRING("71\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<int8_t>('G'), obd::com::Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("47\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<int8_t>('G'), obd::com::Format::Binary);
    TEST_ASSERT_EQUAL_STRING("01000111\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<uint8_t>('G'), obd::com::Format::Decimal);
    TEST_ASSERT_EQUAL_STRING("71\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<uint8_t>('G'), obd::com::Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("47\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<uint8_t>('G'), obd::com::Format::Binary);
    TEST_ASSERT_EQUAL_STRING("01000111\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<int16_t>('G'), obd::com::Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("0047\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<int16_t>('G'), obd::com::Format::Binary);
    TEST_ASSERT_EQUAL_STRING("0000000001000111\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<uint16_t>('G'), obd::com::Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("0047\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<uint16_t>('G'), obd::com::Format::Binary);
    TEST_ASSERT_EQUAL_STRING("0000000001000111\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<int32_t>('G'), obd::com::Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("00000047\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<uint32_t>('G'), obd::com::Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("00000047\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<int64_t>('G'), obd::com::Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("0000000000000047\n",strOut.str().c_str());
    strOut.str(std::string());
    out.println(static_cast<uint64_t>('G'), obd::com::Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("0000000000000047\n",strOut.str().c_str());
    strOut.str(std::string());

    out.print(static_cast<int8_t>('G'), obd::com::Format(-1));
    TEST_ASSERT_EQUAL_STRING("",strOut.str().c_str());
    out.print(static_cast<uint8_t>('G'), obd::com::Format(-1));
    TEST_ASSERT_EQUAL_STRING("",strOut.str().c_str());
    out.print(static_cast<int16_t>('G'), obd::com::Format(-1));
    TEST_ASSERT_EQUAL_STRING("",strOut.str().c_str());
    out.print(static_cast<uint16_t>('G'), obd::com::Format(-1));
    TEST_ASSERT_EQUAL_STRING("",strOut.str().c_str());

    std::cout.rdbuf(oldCoutStreamBuf);
#endif
}

void test_all() {
    UNITY_BEGIN();
    RUN_TEST(test_output);
    UNITY_END();
}