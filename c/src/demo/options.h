/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef DEMO_OPTIONS_H
#define DEMO_OPTIONS_H

#include <stdbool.h>

struct options_s {
  char *router_endpoint;
  char print_serialized;
  int connect_timeout;
  bool print_config;
  bool be_terse;
};

typedef struct options_s options_t;

#endif // DEMO_OPTIONS_H

// vim: set ts=2 sw=2 tw=72 expandtab:
