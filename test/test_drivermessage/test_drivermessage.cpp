/**
 * @file test_data.cpp
 * @author argawaen
 * @date 12/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"
#include "core/driver/Message.h"

using namespace obd::core::driver;

void test_creation() {
    Message message1(14, 78);
    TEST_ASSERT_EQUAL(14, message1.getSource());
    TEST_ASSERT_EQUAL(78, message1.getDestination());
    TEST_ASSERT_EQUAL_STRING("", message1.getMessage().c_str());
    TEST_ASSERT_EQUAL(Message::MessageType::Message, message1.getType());
    TEST_ASSERT(message1.empty())
    TEST_ASSERT_FALSE(message1.hasParams())
    TEST_ASSERT_EQUAL_STRING("", message1.getBaseCommand().c_str());
    TEST_ASSERT(message1.getParams().empty())
    Message message2(14, 78, "good eating", Message::MessageType::Reply);
    TEST_ASSERT_EQUAL(14, message2.getSource());
    TEST_ASSERT_EQUAL(78, message2.getDestination());
    TEST_ASSERT_EQUAL_STRING("good eating", message2.getMessage().c_str());
    TEST_ASSERT_EQUAL(Message::MessageType::Reply, message2.getType());
    TEST_ASSERT_FALSE(message2.empty())
    TEST_ASSERT(message2.hasParams())
    TEST_ASSERT_EQUAL_STRING("good", message2.getBaseCommand().c_str());
    TEST_ASSERT_EQUAL(1, message2.getParams().size());
    TEST_ASSERT_EQUAL_STRING("eating", message2.getParams()[0].c_str());
}

void test_changes() {
    Message message(14, 78, "good eating", Message::MessageType::Reply);
    message.setType(Message::MessageType::Error);
    message.setMessage("toto lives here");
    TEST_ASSERT_EQUAL_STRING("toto", message.getBaseCommand().c_str());
    TEST_ASSERT_EQUAL(Message::MessageType::Error, message.getType());
    message.setType(Message::MessageType::Warning);
    message.clear();
    TEST_ASSERT_EQUAL_STRING("", message.getBaseCommand().c_str());
    TEST_ASSERT_EQUAL(Message::MessageType::Warning, message.getType());
}

void test_printSmallInt() {
    Message message(14, 78);
    message.println(static_cast<uint8_t>('G'));
    TEST_ASSERT_EQUAL_STRING("G\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<uint8_t>('G'), Format::Decimal);
    TEST_ASSERT_EQUAL_STRING("71\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<uint8_t>('G'), Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("47\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<uint8_t>('G'), Format::Binary);
    TEST_ASSERT_EQUAL_STRING("01000111\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<uint8_t>('G'), Format{-1});
    TEST_ASSERT_EQUAL_STRING("\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int8_t>('G'));
    TEST_ASSERT_EQUAL_STRING("G\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int8_t>('G'), Format::Decimal);
    TEST_ASSERT_EQUAL_STRING("71\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int8_t>('G'), Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("47\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int8_t>('G'), Format::Binary);
    TEST_ASSERT_EQUAL_STRING("01000111\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int8_t>('G'), Format{-1});
    TEST_ASSERT_EQUAL_STRING("\n", message.getMessage().c_str());
    message.clear();
}

void test_printLargerInt() {
    Message message(14, 78);

    message.println(static_cast<uint16_t>('G'));
    TEST_ASSERT_EQUAL_STRING("71\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int16_t>('G'));
    TEST_ASSERT_EQUAL_STRING("71\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<uint16_t>('G'), Format{-1});
    TEST_ASSERT_EQUAL_STRING("\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int16_t>('G'), Format{-1});
    TEST_ASSERT_EQUAL_STRING("\n", message.getMessage().c_str());
    message.clear();

    message.println(static_cast<uint32_t>('G'));
    TEST_ASSERT_EQUAL_STRING("71\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<uint32_t>('G'), Format::Decimal);
    TEST_ASSERT_EQUAL_STRING("71\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<uint32_t>('G'), Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("00000047\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<uint32_t>('G'), Format::Binary);
    TEST_ASSERT_EQUAL_STRING("00000000000000000000000001000111\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<uint32_t>('G'), Format{-1});
    TEST_ASSERT_EQUAL_STRING("\n", message.getMessage().c_str());
    message.clear();

    message.println(static_cast<int32_t>('G'));
    TEST_ASSERT_EQUAL_STRING("71\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int32_t>('G'), Format::Decimal);
    TEST_ASSERT_EQUAL_STRING("71\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int32_t>('G'), Format::Hexadecimal);
    TEST_ASSERT_EQUAL_STRING("00000047\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int32_t>('G'), Format::Binary);
    TEST_ASSERT_EQUAL_STRING("00000000000000000000000001000111\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int32_t>('G'), Format{-1});
    TEST_ASSERT_EQUAL_STRING("\n", message.getMessage().c_str());
    message.clear();

    message.println(static_cast<uint64_t>('G'));
    TEST_ASSERT_EQUAL_STRING("71\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int64_t>('G'));
    TEST_ASSERT_EQUAL_STRING("71\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<uint64_t>('G'), Format{-1});
    TEST_ASSERT_EQUAL_STRING("\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int64_t>('G'), Format{-1});
    TEST_ASSERT_EQUAL_STRING("\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<int64_t>('G'), Format::Binary);
    TEST_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000001000111\n", message.getMessage().c_str());
    message.clear();
    message.println(static_cast<uint64_t>('G'), Format::Binary);
    TEST_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000001000111\n", message.getMessage().c_str());
    message.clear();
    message.clear();
    message.printlnBool(true);
    TEST_ASSERT_EQUAL_STRING("true\n", message.getMessage().c_str());
    message.clear();
    message.printlnBool(false);
    TEST_ASSERT_EQUAL_STRING("false\n", message.getMessage().c_str());
    message.clear();
}

void test_printDouble() {
    Message message(14, 78);
    message.println(3.14159, 3);
    TEST_ASSERT_EQUAL_STRING("3.142\n", message.getMessage().c_str());
    message.clear();
    message.println(3.14159, 8);
    TEST_ASSERT_EQUAL_STRING("3.14159000\n", message.getMessage().c_str());
    message.clear();
}

void test_printStrings() {
    Message message(14, 78);
    OString testString{"Hello world"};
    message.println(testString);
    TEST_ASSERT_EQUAL_STRING((testString + "\n").c_str(), message.getMessage().c_str());
    message.clear();
    message.println(testString.c_str());
    TEST_ASSERT_EQUAL_STRING((testString + "\n").c_str(), message.getMessage().c_str());
    message.clear();
}

void test_all() {
    UNITY_BEGIN();
    RUN_TEST(test_creation);
    RUN_TEST(test_changes);
    RUN_TEST(test_printSmallInt);
    RUN_TEST(test_printLargerInt);
    RUN_TEST(test_printDouble);
    RUN_TEST(test_printStrings);
    UNITY_END();
}
