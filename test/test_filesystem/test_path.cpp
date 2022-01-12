/**
 * @file test_path.cpp
 * @author argawaen
 * @date 08/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_helper.h"
#include "fs/Path.h"

using namespace obd::fs;

void test_path() {
  Path p;
  TEST_ASSERT(p.isAbsolute());
  TEST_ASSERT_EQUAL_STRING("/", p.toString().c_str());
  p /= "toto";
  TEST_ASSERT_EQUAL_STRING("/toto", p.toString().c_str());
  TEST_ASSERT_EQUAL_STRING("toto", p.name().c_str());
  TEST_ASSERT_EQUAL_STRING("toto", p.baseName().c_str());
  TEST_ASSERT_EQUAL_STRING("", p.suffix().c_str());
  TEST_ASSERT_EQUAL_STRING("", p.suffixes().c_str());
  TEST_ASSERT_EQUAL_STRING("/", p.parent().toString().c_str());
  p /= Path("gourou.tar.gz");
  TEST_ASSERT_EQUAL_STRING("/toto/gourou.tar.gz", p.toString().c_str());
  TEST_ASSERT_EQUAL_STRING("gourou.tar.gz", p.name().c_str());
  TEST_ASSERT_EQUAL_STRING("gourou", p.baseName().c_str());
  TEST_ASSERT_EQUAL_STRING(".gz", p.suffix().c_str());
  TEST_ASSERT_EQUAL_STRING(".tar.gz", p.suffixes().c_str());
  TEST_ASSERT_EQUAL_STRING("/toto", p.parent().toString().c_str());
  p = "/toto";
  Path p2("bob.odb");
  TEST_ASSERT(!p2.isAbsolute());
  p2.makeAbsolute(p);
  TEST_ASSERT_EQUAL_STRING("/toto/bob.odb", p2.toString().c_str());
}

void test_compact_absolute_path() {
  Path p{"/"};
  p.compact();
  TEST_ASSERT_EQUAL_STRING("/", p.toString().c_str());

  p = "/.";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("/", p.toString().c_str());

  p = "/..";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("/", p.toString().c_str());

  p = "/./..";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("/", p.toString().c_str());

  p = "/../.";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("/", p.toString().c_str());

  p = "/../toto";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("/toto", p.toString().c_str());

  p = "/toto/..";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("/", p.toString().c_str());

  p = "/toto/.";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("/toto", p.toString().c_str());

  p = "/./toto/..";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("/", p.toString().c_str());

  p = "/toto/../titi";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("/titi", p.toString().c_str());

  p = "/toto/./titi";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("/toto/titi", p.toString().c_str());
}

void test_compact_relative_path() {
  Path p{"."};
  p.compact();
  TEST_ASSERT_EQUAL_STRING(".", p.toString().c_str());

  p = "..";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("..", p.toString().c_str());

  p = "./..";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("./..", p.toString().c_str());

  p = "../.";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("..", p.toString().c_str());

  p = "../toto";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("../toto", p.toString().c_str());

  p = "toto/..";
  p.compact();
  TEST_ASSERT_EQUAL_STRING(".", p.toString().c_str());

  p = "toto/.";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("toto", p.toString().c_str());

  p = "./toto/..";
  p.compact();
  TEST_ASSERT_EQUAL_STRING(".", p.toString().c_str());

  p = "toto/../titi";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("titi", p.toString().c_str());

  p = "toto/./titi";
  p.compact();
  TEST_ASSERT_EQUAL_STRING("toto/titi", p.toString().c_str());
}

void test_operator_path() {
  Path p;
  p /= "toto";
  Path p2{"bibi"};
  Path p3 = p / p2 / "tudor";
  TEST_ASSERT_EQUAL_STRING("/toto/bibi/tudor", p3.toString().c_str());
  TEST_ASSERT_EQUAL_STRING("/toto", (p3 / ".." / "..").toString().c_str());
  p2 /= Path{"bob"};
  TEST_ASSERT_EQUAL_STRING("bibi/bob", p2.toString().c_str());

  p2 = Path{"bibi"};
  p2 /= Path{"bibi"};
  TEST_ASSERT_EQUAL_STRING("bibi/bibi", p2.toString().c_str());
}

void test_parent_path() {
  Path p_root;
  TEST_ASSERT_EQUAL_STRING("/", p_root.parent().toString().c_str());
  Path p_relative_1{"toto"};
  TEST_ASSERT_EQUAL_STRING("toto", p_relative_1.parent().toString().c_str());
  Path p_relative_2{"toto/titi"};
  TEST_ASSERT_EQUAL_STRING("toto", p_relative_2.parent().toString().c_str());
}

void test_make_relative_path() {
  Path p2{"bibi/toto"};
  p2.makeRelative(Path{"/bibi"});
  // if one of the base and relative, do nothing
  TEST_ASSERT_EQUAL_STRING("bibi/toto", p2.toString().c_str());

  p2 = Path{"/toto/titi/tutu"};
  p2.makeRelative(Path{"bibi"});
  TEST_ASSERT_EQUAL_STRING("/toto/titi/tutu", p2.toString().c_str());

  p2 = Path{"/toto/titi/tutu"};
  p2.makeRelative(Path{"/toto"});
  TEST_ASSERT_EQUAL_STRING("titi/tutu", p2.toString().c_str());
  p2 = Path{"/toto/titi/tutu"};
  p2.makeRelative(Path{"/toto/tata"});
  TEST_ASSERT_EQUAL_STRING("../titi/tutu", p2.toString().c_str());
  p2 = Path{"/toto/titi/tutu"};
  p2.makeRelative(Path{"/toto/tata/tutu"});
  TEST_ASSERT_EQUAL_STRING("../../titi/tutu", p2.toString().c_str());
  p2 = Path{"/toto.txt"};
  p2.makeRelative(Path{"/"});
  TEST_ASSERT_EQUAL_STRING("toto.txt", p2.toString().c_str());
}
