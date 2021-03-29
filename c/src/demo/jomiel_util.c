/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "demo/jomiel_util.h"
#include "demo/types.h"

#include <stdarg.h>
#include <stdio.h>

static void print_status(Jomiel const self, char const *format, ...) {
  if (self.opts.be_terse == true) {
    return;
  }
  va_list args;
  fprintf(stderr, "status: ");
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
}

Jomiel jutil_connect(Jomiel self) {
  char const *re = self.opts.router_endpoint;
  int const to = self.opts.connect_timeout;

  print_status(self, "<connect> %s (timeout=%d)", re, to);
  self.result = (zsock_connect(self.sck, "%s", re)) != -1;

  if (self.result == false) {
    fprintf(stderr, "error: zsock_connect(): unable to connect\n");
  }
  return self;
}

static void to_hex(Jomiel const self, char const *prefix,
                   uint8_t const *data, size_t const len) {
  if (self.opts.print_serialized == false) {
    return;
  }
  fprintf(stderr, "%s: [%ld] ", prefix, len);
  for (size_t i = 0; i < len; ++i) {
    fprintf(stderr, "%02X", data[i]);
  }
  fprintf(stderr, "\n");
}

static void print_inquiry(Jomiel const self, char const *status,
                          JomielInquiry const msg) {
  print_status(self, status);
  printf("media <\n"
         "  input_uri: \"%s\"\n"
         ">\n",
         msg.media->input_uri);
}

static void *inquiry_new(Jomiel const self, char const *uri,
                         size_t *len) {
  JomielMediaInquiry media = jomiel_media_inquiry_init;
  media.input_uri = (char *)uri;

  JomielInquiry inquiry = jomiel_inquiry_init;
  inquiry.inquiry_case = jomiel_inquiry_case_media;
  inquiry.media = &media;

  *len = jomiel_inquiry_packed_size(&inquiry);
  void *bytes = calloc(1, *len);

  jomiel_inquiry_pack(&inquiry, bytes);

  to_hex(self, "send", (uint8_t *)bytes, *len);
  print_inquiry(self, "<send>", inquiry);

  return bytes;
}

Jomiel jutil_send_inquiry(Jomiel self) {
  char const *uri = poptGetArg(self.opts.ctx);

  size_t len;
  void *bytes = inquiry_new(self, uri, &len);

  zframe_t *frame = zframe_new(bytes, len);
  zmsg_t *msg = zmsg_new();
  zmsg_prepend(msg, &frame);
  free(bytes);

  int const send_result = zmsg_send(&msg, self.sck);
  self.result = (send_result == 0);

  if (self.result == false) {
    fprintf(stderr, "error: zmsg_send(): send_result=%d\n",
            send_result);
  }
  zframe_destroy(&frame);
  zmsg_destroy(&msg);

  return self;
}

static void dump_terse_response(JomielResponse const *msg) {
  printf("---\ntitle: %s\nquality:\n", msg->media->title);
  for (size_t i = 0; i < msg->media->n_stream; ++i) {
    JomielStreamQuality const *qty = msg->media->stream[i]->quality;
    printf("  profile: %s\n    width: %d\n    height: %d\n",
           qty->profile, qty->width, qty->height);
  }
}

static void foreach_stream(JomielMediaResponse const *msg) {
  for (size_t i = 0; i < msg->n_stream; ++i) {
    JomielStreamQuality const *qty = msg->stream[i]->quality;
    printf("stream {\n"
           "  uri: \"%s\"\n"
           "  quality {\n"
           "    profile: \"%s\"\n    width: %d\n    height: %d\n"
           "  }\n" // quality
           "}\n",  // stream
           msg->stream[i]->uri, qty->profile, qty->width, qty->height);
  }
}

static void print_failed(JomielResponse const *msg) {
  printf("status <\n"
         "  code: %d\n"
         "  error: %d\n"
         "  http <\n"
         "    code: %d\n"
         "  >\n"
         "  message: \"%s\"\n"
         ">\n",
         msg->status->code, msg->status->error, msg->status->http->code,
         msg->status->message);
}

static void print_message(Jomiel const self, char const *status,
                          JomielResponse const *msg) {
  print_status(self, status);

  if (msg->status->code == JOMIEL_STATUS_CODE_ok) {
    printf("title: \"%s\"\n"
           "identifier: \"%s\"\n",
           msg->media->title, msg->media->identifier);
    foreach_stream(msg->media);
  } else {
    print_failed(msg);
  }
}

static void dump_response(Jomiel const self,
                          JomielResponse const *msg) {
  static char const status[] = "<recv>";

  if (msg->status->code == JOMIEL_STATUS_CODE_ok) {
    (self.opts.be_terse == true) ? dump_terse_response(msg)
                                 : print_message(self, status, msg);
  } else {
    print_message(self, status, msg);
  }
}

static bool poll_receive_message(Jomiel const self, zmsg_t **zmsg) {
  *zmsg = zmsg_recv(self.sck);
  bool const result = (zmsg != NULL);

  if (result == false) {
    fprintf(stderr, "error: zmsg_recv(): interrupted\n");
  }

  return result;
}

static bool poll_unpack_message(Jomiel const self, zmsg_t *zmsg,
                                JomielResponse **msg) {
  zframe_t *zfrm = zmsg_pop(zmsg);
  if (zfrm == NULL) {
    fprintf(stderr, "error: zmq_pop(): returned NULL\n");
    return false;
  }

  uint8_t const *bytes = zframe_data(zfrm);
  size_t const len = zframe_size(zfrm);
  to_hex(self, "recv", bytes, len);

  *msg = jomiel_response_unpack(NULL, len, bytes);
  zframe_destroy(&zfrm);

  return (*msg != NULL);
}

static Jomiel poll_message(Jomiel self) {
  zmsg_t *zmsg = NULL;
  self.result = false;

  if (poll_receive_message(self, &zmsg) == true) {
    JomielResponse *msg = NULL;

    if (poll_unpack_message(self, zmsg, &msg) == true) {
      dump_response(self, msg);
      jomiel_response_free_unpacked(msg, NULL);
      self.result = true;
    }
  }

  zmsg_destroy(&zmsg);
  return self;
}

Jomiel jutil_receive_response(Jomiel self) {
  if (self.result == false) {
    return self;
  }

  zpoller_t *poller = zpoller_new(self.sck, NULL);
  zpoller_wait(poller, self.opts.connect_timeout * 1000);

  self.result = false;

  if (zpoller_expired(poller) == true) {
    fprintf(stderr, "error: zpoller_expired(): connection timed out\n");
  } else if (zpoller_terminated(poller) == true) {
    fprintf(stderr, "error: zpoller_terminated(): interrupted\n");
  } else {
    self = poll_message(self);
  }

  zpoller_destroy(&poller);
  return self;
}
