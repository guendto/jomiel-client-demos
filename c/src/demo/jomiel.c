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

#include "demo/jomiel.h"
#include "demo/jomiel_util.h"

#include <stdio.h>


static inline void print_status(const jomiel_t *self,
                                const char *format, ...) {
  if (!self->opts->be_terse) {
    va_list args;
    fprintf(stderr, "status: ");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
  }
}

jomiel_t *jomiel_new(options_t *opts) {
  jomiel_t *self = (jomiel_t *)calloc(1, sizeof(struct jomiel_s));

  self->opts = opts;
  self->sck = zsock_new(ZMQ_REQ);

  if (self->sck == NULL) {
    fprintf(stderr, "error: failed to create a new socket\n");
    jomiel_destroy(&self);
    return NULL;
  }

  print_status(self, "<connect> %s (timeout=%d)", opts->router_endpoint,
               opts->connect_timeout);

  const int rc = zsock_connect(self->sck, "%s", opts->router_endpoint);

  if (rc == -1) {
    fprintf(stderr, "error: failed to connect; invalid endpoint\n");
    jomiel_destroy(&self);
    return NULL;
  }

  zsock_set_linger(self->sck, 0);
  return self;
}

void jomiel_destroy(jomiel_t **self_p) {
  if (*self_p) {
    jomiel_t *self = *self_p;
    zsock_destroy(&self->sck);
    free(self);
  }
}

static inline void to_hex(const jomiel_t *self, const char *prefix,
                          const uint8_t *data, const size_t len) {
  if (self->opts->print_serialized) {
    fprintf(stderr, "%s: [%ld] ", prefix, len);
    for (int i = 0; i < len; ++i)
      fprintf(stderr, "%02X", data[i]);
    fprintf(stderr, "\n");
  }
}

static inline void *inquiry_new(const jomiel_t *self, char *uri,
                                size_t *len) {
  Jomiel__Protobuf__V1beta1__MediaInquiry media =
      JOMIEL__PROTOBUF__V1BETA1__MEDIA_INQUIRY__INIT;

  Jomiel__Protobuf__V1beta1__Inquiry inquiry =
      JOMIEL__PROTOBUF__V1BETA1__INQUIRY__INIT;

  inquiry.inquiry_case =
      JOMIEL__PROTOBUF__V1BETA1__INQUIRY__INQUIRY_MEDIA;

  inquiry.media = &media;
  media.input_uri = uri;

  *len = jomiel__protobuf__v1beta1__inquiry__get_packed_size(&inquiry);

  /* Build a serialized string. */
  void *bytes = (void *)calloc(1, *len);
  jomiel__protobuf__v1beta1__inquiry__pack(&inquiry, bytes);

  return bytes;
}

static inline int jomiel_send(const jomiel_t *self, char *uri) {
  size_t len;

  void *bytes = inquiry_new(self, uri, &len);
  to_hex(self, "send", (uint8_t *)bytes, len);

  print_status(self, "<send>");

  zmsg_t *msg = zmsg_new();
  zframe_t *frame = zframe_new(bytes, len);

  zmsg_prepend(msg, &frame);
  free(bytes);

  const int rc = zmsg_send(&msg, self->sck);

  zframe_destroy(&frame);
  zmsg_destroy(&msg);

  if (rc != 0)
    fprintf(stderr, "error: zmsg_send: rc=%d\n", rc);

  return (rc == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static inline void dump_terse_response(
    const Jomiel__Protobuf__V1beta1__Response *message) {
  const Jomiel__Protobuf__V1beta1__MediaResponse *media_response =
      message->media;
  const Jomiel__Protobuf__V1beta1__MediaResponse__Stream__StreamQuality
      *quality;
  int i;

  printf("---\ntitle: %s\n"
         "quality:\n",
         media_response->title);

  for (i = 0; i < media_response->n_stream; ++i) {
    quality = media_response->stream[i]->quality;
    printf("  profile: %s\n    width: %d\n    height: %d\n",
           quality->profile, quality->width, quality->height);
  }
}

static inline void
foreach_stream(const Jomiel__Protobuf__V1beta1__MediaResponse *media) {
  const Jomiel__Protobuf__V1beta1__MediaResponse__Stream__StreamQuality
      *quality;
  const Jomiel__Protobuf__V1beta1__MediaResponse__Stream *stream;
  int i;
  for (i = 0; i < media->n_stream; ++i) {
    stream = media->stream[i];
    quality = stream->quality;

    printf("stream {\n");
    printf("  uri: \"%s\"\n", stream->uri);
    printf("  quality {\n");
    printf("    profile: \"%s\"\n    width: %d\n    height: %d\n",
           quality->profile, quality->width, quality->height);
    printf("  }\n"); // quality
    printf("}\n");   // stream
  }
}

static inline void
print_message(const jomiel_t *self, const char *status,
              const Jomiel__Protobuf__V1beta1__Response *message) {
  print_status(self, status);
  if (message->status->code !=
      JOMIEL__PROTOBUF__V1BETA1__STATUS_CODE__STATUS_CODE_OK) {
    fprintf(stderr,
            "failed: code: %d, error: %d, http: %d\n    msg: '%s'\n",
            message->status->code, message->status->error,
            message->status->http->code, message->status->message);

  } else {
    const Jomiel__Protobuf__V1beta1__MediaResponse *media =
        message->media;

    printf("title: \"%s\"\n", media->title);
    printf("identifier: \"%s\"\n", media->identifier);

    foreach_stream(message->media);
  }
}

static inline void
dump_response(const jomiel_t *self,
              const Jomiel__Protobuf__V1beta1__Response *response) {
  if (response->status->code ==
      JOMIEL__PROTOBUF__V1BETA1__STATUS_CODE__STATUS_CODE_OK) {
    if (self->opts->be_terse)
      dump_terse_response(response);
    else
      print_message(self, "<recv>", response);
  } else {
    print_message(self, "<recv>", response);
  }
}

static inline int recv_response(const jomiel_t *self) {
  zmsg_t *msg = zmsg_recv(self->sck);
  zframe_t *frame = zmsg_pop(msg);

  const uint8_t *bytes = zframe_data(frame); // Serialized string.
  const size_t len = zframe_size(frame);

  Jomiel__Protobuf__V1beta1__Response *response =
      jomiel__protobuf__v1beta1__response__unpack(NULL, len, bytes);

  int rc = EXIT_FAILURE;

  if (response != NULL) {
    to_hex(self, "recv", bytes, len);
    dump_response(self, response);
    jomiel__protobuf__v1beta1__response__free_unpacked(response, NULL);
    rc = EXIT_SUCCESS;
  } else {
    fprintf(stderr, "error: unpacking serialized string failed\n");
  }

  zframe_destroy(&frame);
  zmsg_destroy(&msg);

  return rc;
}

static inline int jomiel_recv(const jomiel_t *self) {
  zpoller_t *poller = zpoller_new(self->sck, NULL);

  zpoller_wait(poller, self->opts->connect_timeout * 1000);

  int rc = EXIT_FAILURE;

  if (zpoller_expired(poller)) {
    fprintf(stderr, "error: connection timed out\n");
  } else if (zpoller_terminated(poller)) {
    fprintf(stderr, "error: interrupted\n");
  } else {
    rc = recv_response(self);
  }
  zpoller_destroy(&poller);
  return rc;
}

int jomiel_inquire(const jomiel_t *self, char *uri) {
  int rc = jomiel_send(self, uri);
  if (rc == EXIT_SUCCESS)
    rc = jomiel_recv(self);
  return rc;
}

// vim: set ts=2 sw=2 tw=72 expandtab:
