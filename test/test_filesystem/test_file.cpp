/**
 * @file test_file.cpp
 * @author argawaen
 * @date 09/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "../test_helper.h"
#include "fs/File.h"
#include "fs/FileSystem.h"
#include "fs/ConfigFile.h"

using namespace obd::fs;

void test_file() {
    std::shared_ptr<FileSystem> hdd = baseSys.getNode<FileSystem>();
    TextFile file(hdd, Path{"/toto.txt"}, ios::out);
    file.write("Coucou!!");
    file.write('1');
    file.close();
    TEST_ASSERT(hdd->exists(Path{"/toto.txt"}));
    TextFile f_read(hdd);
    TEST_ASSERT(f_read.open(Path{"/toto.txt"}, ios::in))
    TEST_ASSERT_FALSE(f_read.open(Path{"/toto.txt"}, ios::in))
    auto line = f_read.readLine();
    f_read.close();
    f_read.close();
    TEST_ASSERT_EQUAL_STRING("Coucou!!1", line.c_str());
    f_read.write(' ');
    f_read.write("");
    char readChar = f_read.read();
    TEST_ASSERT_EQUAL_STRING("\0", &readChar);
    TEST_ASSERT_EQUAL_STRING("", f_read.readLine().c_str());
    TEST_ASSERT_FALSE(file.open(Path{"/toto.txt"}, ios::none))
    TEST_ASSERT(file.open(Path{"/toto.txt"}, ios::app))
    file.write('\n');
    file.write("Coucou!!");
    file.close();
    TEST_ASSERT(f_read.open(Path{"/toto.txt"}, ios::in))
    auto line1 = f_read.readLine();
    auto line2 = f_read.readLine(3);

    TEST_ASSERT_EQUAL_STRING("Coucou!!1\n", line1.c_str());
    TEST_ASSERT_EQUAL_STRING("Cou", line2.c_str());
}

void test_file_read() {
    std::shared_ptr<FileSystem> hdd = baseSys.getNode<FileSystem>();
    TextFile file(hdd, Path{"/toto.txt"}, ios::in);
    TEST_ASSERT(file.available());
    file.close();
}

void test_config_file(){
    auto hdd = baseSys.getNode<FileSystem>();
    // testing bad things
    ConfigFile bad_config(nullptr);
    bad_config.saveConfig("TestDriver");
    TEST_ASSERT_FALSE(bad_config.configExists("TestDriver"))
    ConfigFile config(hdd);
    config.saveConfig("TestDriver");
    config.loadConfig("TestDriver");
    TEST_ASSERT_FALSE(config.configExists("TestDriver"))
    // populate fake parameters
    config.addConfigParameter("parameter1", "Value1");
    config.addConfigParameter("parameter2", "Value2");
    config.addConfigParameter("parameter3", "Value3   ");
    TEST_ASSERT_EQUAL_STRING("",config.getKey("parameter4").c_str());
    TEST_ASSERT_EQUAL_STRING("Value3   ",config.getKey("parameter3").c_str());
    config.saveConfig("TestDriver");
    config.clear();
    config.loadConfig("TestDriver");
    TEST_ASSERT_EQUAL_STRING("Value3",config.getKey("parameter3").c_str());
    TEST_ASSERT(hdd->rm(Path{"/config/TestDriver.cfg"}))
}
