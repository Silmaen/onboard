/**
 * @file test_base.h
 * @author argawaen
 * @date 09/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "com/Communicator.h"
#include "core/FileSystem.h"
#include "test_helper.h"

void test_all();

void setup() {
  Port.begin(obd::com::Verbosity::Trace, true);
  fs.begin();
  test_all();
  fs.end();
}

void loop() {}
