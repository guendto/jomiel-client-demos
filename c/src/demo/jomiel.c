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
#include "demo/jomiel_util.h"

#include <stdio.h>

static Jomiel init(Options const opts) {
  Jomiel self = {.result = false, .opts = opts, .sck = NULL};

  self.sck = zsock_new(ZMQ_REQ);
  if (self.sck == NULL) {
    fprintf(stderr, "error: zsock(): unable to create a socket\n");
    return self;
  }

  zsock_set_linger(self.sck, 0);
  self = jutil_connect(self);

  return self;
}

static bool has_more(Jomiel const self) {
  return (poptPeekArg(self.opts.ctx) != NULL && self.result == true);
}

static Jomiel inquire(Jomiel self) {
  while (has_more(self) == true) {
    self = jutil_send_inquiry(self);
    self = jutil_receive_response(self);
  }
  return self;
}

static void cleanup(Jomiel self) {
  self.opts.ctx = poptFreeContext(self.opts.ctx);
  zsock_destroy(&(self.sck));
}

int jomiel_run(Options const opts) {
  Jomiel jomiel = init(opts);
  jomiel = inquire(jomiel);
  cleanup(jomiel);
  return (jomiel.result == true) ? EXIT_SUCCESS : EXIT_FAILURE;
}
