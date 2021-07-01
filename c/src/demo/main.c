/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2019-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "demo/jomiel.h"

int main(int argc, char const **argv) {
  Options const opts = parse_options(argc, argv);
  return jomiel_run(opts);
}
