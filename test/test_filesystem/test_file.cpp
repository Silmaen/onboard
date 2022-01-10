/**
 * @file test_file.cpp
 * @author argawaen
 * @date 09/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "../test_helper.h"
#include "core/File.h"
#include "core/FileSystem.h"

using namespace obd::core;

void test_file() {
  TextFile file(Path{"/toto.txt"}, ios::out);
  file.write("Coucou!!");
  file.write('1');
  file.close();
  TEST_ASSERT(fs.exists(Path{"/toto.txt"}));
  TextFile f_read;
  TEST_ASSERT(f_read.open(Path{"/toto.txt"}, ios::in))
  TEST_ASSERT_FALSE(f_read.open(Path{"/toto.txt"}, ios::in))
  auto l = f_read.readLine();
  f_read.close();
  f_read.close();
  TEST_ASSERT_EQUAL_STRING("Coucou!!1", l.c_str());
  f_read.write(' ');
  f_read.write("");
  char a = f_read.read();
  TEST_ASSERT_EQUAL_STRING("\0", &a);
  TEST_ASSERT_EQUAL_STRING("", f_read.readLine().c_str());
  TEST_ASSERT_FALSE(file.open(Path{"/toto.txt"}, ios::none))
  TEST_ASSERT(file.open(Path{"/toto.txt"}, ios::app))
  file.write('\n');
  file.write("Coucou!!");
  file.close();
  TEST_ASSERT(f_read.open(Path{"/toto.txt"}, ios::in))
  auto l1 = f_read.readLine();
  auto l2 = f_read.readLine(3);

  TEST_ASSERT_EQUAL_STRING("Coucou!!1\n", l1.c_str());
  TEST_ASSERT_EQUAL_STRING("Cou", l2.c_str());
}
