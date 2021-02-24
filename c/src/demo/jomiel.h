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

#include <czmq.h>
#include "demo/options.h"

struct jomiel_s {
  options_t *opts;
  zsock_t *sck;
};

typedef struct jomiel_s jomiel_t;

jomiel_t *jomiel_new(options_t *);
void jomiel_destroy(jomiel_t **);

int jomiel_inquire(const jomiel_t *, char *);

// vim: set ts=2 sw=2 tw=72 expandtab:
