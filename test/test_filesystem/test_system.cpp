/**
 * @file test_system.cpp
 * @author argawaen
 * @date 08/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "../test_helper.h"
#include "fs/FileSystem.h"

using namespace obd::fs;

void test_uninitialized() {
    FileSystem hdd(nullptr);
    TEST_ASSERT_FALSE(hdd.exists(Path("/title.odb")))
    TEST_ASSERT_FALSE(hdd.isDir(Path("/title.odb")))
    TEST_ASSERT_FALSE(hdd.isFile(Path("/title.odb")))
    TEST_ASSERT_FALSE(hdd.touch(Path("/title.odb")))
    TEST_ASSERT_FALSE(hdd.mkdir(Path("/temp")))
    TEST_ASSERT_FALSE(hdd.rm(Path("/temp")))
    TEST_ASSERT_FALSE(hdd.rmdir(Path("/temp")))
    TEST_ASSERT_FALSE(hdd.cd(Path("/temp")))
    TEST_ASSERT(hdd.listDir().empty())
    hdd.printInfo();
    hdd.end();
    TEST_ASSERT_FALSE(hdd.init())
}

void test_filesystem() {
    auto hdd = baseSys.getDriver<FileSystem>();
    hdd->update(0);
    hdd->loadConfigFile();
    hdd->saveConfigFile();
    hdd->printHelp();
    TEST_ASSERT_FALSE(hdd->treatCommand(obd::core::Command()));
    TEST_ASSERT_EQUAL_STRING("/", hdd->cwd().toString().c_str());
    TEST_ASSERT(hdd->exists(Path("/title.odb")))
    TEST_ASSERT(hdd->isFile(Path("/title.odb")))
    auto listing = hdd->listDir(Path{"/title.odb"});
    TEST_ASSERT_EQUAL(0, listing.size());
}

void test_directories() {
    auto hdd = baseSys.getDriver<FileSystem>();
    TEST_ASSERT_FALSE(hdd->touch(Path("/tmp5680234/file.txt")))
    TEST_ASSERT_FALSE(hdd->cd(Path("/tmp5680234")))
    Path tempPath{"/tmp5680234/bob/boby"};
    TEST_ASSERT(hdd->mkdir(tempPath, true))
    TEST_ASSERT(hdd->isDir(tempPath))
    auto listing = hdd->listDir(Path{"/tmp5680234"});
    TEST_ASSERT_EQUAL(1, listing.size());
    TEST_ASSERT_EQUAL_STRING("bob", listing[0].toString().c_str());
    TEST_ASSERT(hdd->cd(Path("/tmp5680234/bob")))
    TEST_ASSERT_EQUAL_STRING("/tmp5680234/bob", hdd->cwd().toString().c_str());
    listing = hdd->listDir(Path{"."});
    TEST_ASSERT_EQUAL(1, listing.size());
    listing = hdd->listDir(Path{""});
    TEST_ASSERT_EQUAL(1, listing.size());
    TEST_ASSERT(hdd->cd(Path("../..")))
    TEST_ASSERT_EQUAL_STRING("/", hdd->cwd().toString().c_str());
    TEST_ASSERT(hdd->rmdir(Path("/tmp5680234/bob"), true))
    TEST_ASSERT(hdd->mkdir(Path("/tmp5680234/bob"), false));
    TEST_ASSERT_FALSE(hdd->rmdir(tempPath, false, false));
    TEST_ASSERT_FALSE(hdd->rmdir(Path("/tmp5680234"), false, false));
    TEST_ASSERT(hdd->rmdir(Path("/tmp5680234"), true))
    TEST_ASSERT_FALSE(hdd->exists(Path{"/tmp5680234"}))
}

void test_create_destroy() {
    auto hdd = baseSys.getDriver<FileSystem>();
    hdd->cd(Path{"/"});
    TEST_ASSERT(hdd->mkdir(Path("tmp5681234")))
    TEST_ASSERT(hdd->mkdir(Path("/tmp5681234/folder")))
    TEST_ASSERT_FALSE(hdd->touch(Path("/tmp5681234/folder")))
    TEST_ASSERT(hdd->touch(Path("/tmp5681234/file.txt")))
    TEST_ASSERT(hdd->touch(Path("/tmp5681234/file.txt")))
    TEST_ASSERT_FALSE(hdd->rm(Path("/tmp5681234/folder")))
    TEST_ASSERT_FALSE(hdd->rmdir(Path("/tmp5681234/file.txt")))
    TEST_ASSERT_FALSE(hdd->mkdir(Path("/tmp5681234/file.txt")))
    TEST_ASSERT(hdd->mkdir(Path("/tmp5681234/folder"), false, true))
    TEST_ASSERT_FALSE(hdd->mkdir(Path("/tmp5681234/folder"), false, false))
    TEST_ASSERT_FALSE(hdd->mkdir(Path("/tmp5681235/folder"), false, false))
    hdd->cd(Path{"/tmp5681234"});
    TEST_ASSERT(hdd->rmdir(Path("folder")))

    TEST_ASSERT(hdd->rm(Path("file.txt")))
    TEST_ASSERT(hdd->touch(Path("file.txt")))
    TEST_ASSERT(hdd->rm(Path("/tmp5681234/file.txt")))
    hdd->cd(Path{"/"});
    TEST_ASSERT(hdd->rmdir(Path("/tmp5681234"), true))
}
