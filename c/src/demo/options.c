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

#include "demo/options.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>

static void exit_with(Options opts, int const exit_status) {
  opts.ctx = poptFreeContext(opts.ctx);
  exit(exit_status);
}

static void exit_failed(Options const opts) {
  exit_with(opts, EXIT_FAILURE);
}
static void exit_ok(Options const opts) {
  exit_with(opts, EXIT_SUCCESS);
}

static bool next_uri(Options const opts) {
  return (poptPeekArg(opts.ctx) != NULL);
}

static void dump_config(Options const opts) {
  puts("---");
#define X(type, name, format)                                          \
  printf("%s: " format "\n", #name, opts.name);
  X_FIELDS
#undef X
  printf("URI = [ ");
  while (next_uri(opts) == true) {
    printf("%s ", poptGetArg(opts.ctx));
  }
  printf("]\n");
  exit_ok(opts);
}

static void print_zmq_version(Options const opts) {
  int major, minor, patch;
  zmq_version(&major, &minor, &patch);
  printf("ZeroMQ version %d.%d.%d\n", major, minor, patch);
  exit_ok(opts);
}

static void parse(Options const opts) {
  int const result = poptGetNextOpt(opts.ctx);
  if (result < -1) {
    fprintf(stderr, "error: bad argument %s: %s\n",
            poptBadOption(opts.ctx, POPT_BADOPTION_NOALIAS),
            poptStrerror(result));
    poptPrintUsage(opts.ctx, stderr, 0);
    exit_failed(opts);
  }
}

Options parse_options(int const argc, char const **argv) {

  Options opts = {.router_endpoint = "tcp://localhost:5514",
                  .connect_timeout = 30};

  struct poptOption const l_opts[] = {
      {.longName = "print-config",
       .shortName = 'D',
       .argInfo = POPT_ARG_NONE,
       .arg = &opts.print_config,
       .descrip = "Print configuration values and exit",
       .argDescrip = NULL},
      {.longName = "version-zmq",
       .shortName = 'V',
       .argInfo = POPT_ARG_NONE,
       .arg = &opts.version_zmq,
       .descrip = "Print ZeroMQ version and exit",
       .argDescrip = NULL},
      {.longName = "router-endpoint",
       .shortName = 'r',
       .argInfo = POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT,
       .arg = &opts.router_endpoint,
       .descrip = "Specify the router endpoint address",
       .argDescrip = "ADDR"},
      {.longName = "connect-timeout",
       .shortName = 't',
       .argInfo = POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT,
       .arg = &opts.connect_timeout,
       .descrip = "Specify maximum time in seconds for the connection "
                  "allowed to take",
       .argDescrip = "TIME"},
      {.longName = "print-serialized",
       .shortName = 's',
       .argInfo = POPT_ARG_NONE,
       .arg = &opts.print_serialized,
       .descrip = "Print serialized messages",
       .argDescrip = NULL},
      {.longName = "be-terse",
       .shortName = 'q',
       .argInfo = POPT_ARG_NONE,
       .arg = &opts.be_terse,
       .descrip =
           "Be brief and to the point; dump interesting details only",
       .argDescrip = NULL},
      POPT_AUTOHELP POPT_TABLEEND};

  opts.ctx = poptGetContext(NULL, argc, argv, l_opts, 0);
  poptSetOtherOptionHelp(opts.ctx, "[options] uri ...");

  parse(opts);

  if (opts.print_config == true) {
    dump_config(opts);
  } else if (opts.version_zmq == true) {
    print_zmq_version(opts);
  } else {
    if (poptPeekArg(opts.ctx) == NULL) {
      fprintf(stderr, "error: input URI not given\n");
      exit_failed(opts);
    }
  }
  return opts;
}
