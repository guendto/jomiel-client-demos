/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "demo/options.h"

#include <czmq.h>

// Structures.

struct Jomiel {
  Options opts;
  zsock_t *sck;
  bool result;
};

typedef struct Jomiel Jomiel;

// Functions.

Jomiel jutil_connect(Jomiel);
Jomiel jutil_send_inquiry(Jomiel);
Jomiel jutil_receive_response(Jomiel);
