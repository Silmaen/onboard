/**
 * @file test_base.h
 * @author argawaen
 * @date 09/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "test_helper.h"

void test_all();

void setup() {
    // adding base nodes to the system
    baseSys.init();
    test_all();
}

void loop() {}
