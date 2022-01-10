/**
 * @file test_system.cpp
 * @author argawaen
 * @date 08/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_helper.h"
#include "core/FileSystem.h"

using namespace obd::core;

void test_uninitialized() {
  fs.end();
  TEST_ASSERT_FALSE(fs.exists(Path("/title.odb")))
  TEST_ASSERT_FALSE(fs.isDir(Path("/title.odb")))
  TEST_ASSERT_FALSE(fs.isFile(Path("/title.odb")))
  TEST_ASSERT_FALSE(fs.mkdir(Path("/temp")))
  TEST_ASSERT_FALSE(fs.rm(Path("/temp")))
  TEST_ASSERT_FALSE(fs.rmdir(Path("/temp")))
  TEST_ASSERT_FALSE(fs.cd(Path("/temp")))
  TEST_ASSERT(fs.listDir().empty())
  TEST_ASSERT(fs.begin())
}

void test_filesystem() {
  TEST_ASSERT_EQUAL_STRING("/", fs.cwd().toString().c_str());
  TEST_ASSERT(fs.exists(Path("/title.odb")))
  TEST_ASSERT(fs.isFile(Path("/title.odb")))
  auto ls = fs.listDir(Path{"/title.odb"});
  TEST_ASSERT_EQUAL(0, ls.size());
}

void test_directories() {
  Path p{"/tmp5680234/bob/boby"};
  TEST_ASSERT(fs.mkdir(p, true))
  TEST_ASSERT(fs.isDir(p))
  auto ls = fs.listDir(Path{"/tmp5680234"});
  TEST_ASSERT_EQUAL(1, ls.size());
  TEST_ASSERT_EQUAL_STRING("bob", ls[0].toString().c_str());
  TEST_ASSERT(fs.cd(Path("/tmp5680234/bob")))
  TEST_ASSERT_EQUAL_STRING("/tmp5680234/bob", fs.cwd().toString().c_str());
  TEST_ASSERT(fs.cd(Path("../..")))
  TEST_ASSERT_EQUAL_STRING("/", fs.cwd().toString().c_str());
  TEST_ASSERT(fs.rmdir(Path("/tmp5680234/bob"), true))
  TEST_ASSERT(fs.mkdir(Path("/tmp5680234/bob"), false));
  TEST_ASSERT_FALSE(fs.rmdir(p, false, false));
  TEST_ASSERT_FALSE(fs.rmdir(Path("/tmp5680234"), false, false));
  TEST_ASSERT(fs.rmdir(Path("/tmp5680234"), true))
  TEST_ASSERT_FALSE(fs.exists(Path{"/tmp5680234"}))
}

void test_create_destroy() {
  fs.cd(Path{"/"});
  TEST_ASSERT(fs.mkdir(Path("tmp5680234")))
  TEST_ASSERT(fs.mkdir(Path("/tmp5680234/folder")))
  TEST_ASSERT_FALSE(fs.touch(Path("/tmp5680234/folder")))
  TEST_ASSERT(fs.touch(Path("/tmp5680234/file.txt")))
  TEST_ASSERT(fs.touch(Path("/tmp5680234/file.txt")))
  TEST_ASSERT_FALSE(fs.rm(Path("/tmp5680234/folder")))
  TEST_ASSERT_FALSE(fs.rmdir(Path("/tmp5680234/file.txt")))

  TEST_ASSERT_FALSE(fs.mkdir(Path("/tmp5680234/file.txt")))
  TEST_ASSERT(fs.mkdir(Path("/tmp5680234/folder"), false, true))
  TEST_ASSERT_FALSE(fs.mkdir(Path("/tmp5680234/folder"), false, false))
  TEST_ASSERT_FALSE(fs.mkdir(Path("/tmp5680235/folder"), false, false))
  fs.cd(Path{"/tmp5680234"});
  TEST_ASSERT(fs.rmdir(Path("folder")))
  auto ls = fs.listDir(Path{"."});
  TEST_ASSERT_EQUAL(1, ls.size());
  TEST_ASSERT(fs.rm(Path("file.txt")))
  TEST_ASSERT(fs.touch(Path("file.txt")))
  TEST_ASSERT(fs.rm(Path("/tmp5680234/file.txt")))
  ls = fs.listDir(Path{""});
  TEST_ASSERT_EQUAL(0, ls.size());

  fs.cd(Path{"/"});
  TEST_ASSERT(fs.rmdir(Path("/tmp5680234"), false))
  TEST_ASSERT_FALSE(fs.touch(Path("/tmp5680234/file.txt")))
  TEST_ASSERT_FALSE(fs.cd(Path("/tmp5680234")))
}
