/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <popt.h>

// Structures.

// https://en.wikipedia.org/wiki/X_Macro
#define X_FIELDS                                                       \
  X(int, print_config, "%d")                                           \
  X(int, version_zmq, "%d")                                            \
  X(char const *, router_endpoint, "%s")                               \
  X(int, connect_timeout, "%d")                                        \
  X(int, print_serialized, "%d")                                       \
  X(int, be_terse, "%d")

struct Options {
  poptContext ctx;
#define X(type, name, format) type name;
  X_FIELDS
#undef X
};

typedef struct Options Options;
