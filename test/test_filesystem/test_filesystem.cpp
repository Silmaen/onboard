/**
 * @file test_filesystem.cpp
 * @author argawaen
 * @date 04/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_base.h"

void test_path();
void test_compact_absolute_path();
void test_compact_relative_path();
void test_operator_path();
void test_parent_path();
void test_make_relative_path();

void test_filesystem();
void test_directories();
void test_uninitialized();
void test_create_destroy();

void test_file();
void test_file_read();
void test_config_file();

void test_all() {
  UNITY_BEGIN();
  // tests on paths
  RUN_TEST(test_path);
  RUN_TEST(test_compact_absolute_path);
  RUN_TEST(test_compact_relative_path);
  RUN_TEST(test_operator_path);
  RUN_TEST(test_parent_path);
  RUN_TEST(test_make_relative_path);
  // tests on file system
  RUN_TEST(test_filesystem);
  RUN_TEST(test_directories);
  RUN_TEST(test_uninitialized);
  RUN_TEST(test_create_destroy);
  // test files
  RUN_TEST(test_file);
  RUN_TEST(test_file_read);
  RUN_TEST(test_config_file);
  UNITY_END();
}
