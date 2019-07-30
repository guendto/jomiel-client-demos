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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "demo/jomiel.h"

static inline int version_zmq() {
  int major, minor, patch;
  zmq_version(&major, &minor, &patch);
  printf("ZeroMQ version %d.%d.%d\n", major, minor, patch);
  return EXIT_SUCCESS;
}

#define R(...) #__VA_ARGS__

static const char usage[] =
  "Usage:\n"
  "    demo [-hDVqj] [-r <addr>] [-t <time>] [URI ...]\n"
  "\n"
  "Options:\n"
  "   -h --help                    Print this help and exit\n"
  "   -D --print-config            Print configuration values and exit\n"
  "   -V --version-zmq             Print ZeroMQ version and exit\n"
  "   -r --router-endpoint <addr>  Specify the router endpoint address\n"
  "   -t --connect-timeout <time>  Specify maximum time in seconds for\n"
  "                                 the connection allowed to take\n"
  "   -s --print-serialized        Print serialized messages\n"
  "   -q --be-terse                Be brief and to the point; dump\n"
  "                                 interesting details only";

static inline int print_usage() {
  puts(usage);
  return EXIT_SUCCESS;
}

static inline void opts_set_defaults(options_t* opts) {
  static const char default_router_endpoint[] = "tcp://localhost:5570";
  opts->router_endpoint = (char*)default_router_endpoint;
  opts->connect_timeout = 30;
}

static inline int print_config(const options_t* opts,
                               const int argc,
                               char** argv) {
  printf(
    "---\n"
    "print-config = %d\n"
    "router-endpoint = %s\n"
    "connect-timeout = %d\n"
    "print-serialized = %d\n"
    "be-terse = %d\n",
    opts->print_config, opts->router_endpoint, opts->connect_timeout,
    opts->print_serialized, opts->be_terse);

  printf("URI = [ ");
  for (int c = 0; c < argc; ++c)
    printf("%s ", argv[c]);
  printf("]\n");

  return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
  static const struct option long_options[] = {
    {"print-config", no_argument, NULL, 'D'},
    {"version-zmq", no_argument, NULL, 'V'},
    {"router-endpoint", required_argument, NULL, 'r'},
    {"connect-timeout", required_argument, NULL, 't'},
    {"print-serialized", no_argument, NULL, 's'},
    {"be-terse", no_argument, NULL, 'q'},
    {0, 0, 0, 0}};

  options_t opts = {0};
  opts_set_defaults(&opts);

  const char opt_str[] = "?hDVr:t:sq";

  int c;
  while ((c = getopt_long(argc, argv, opt_str, long_options, NULL)) !=
         -1) {
    switch (c) {
      case 'h':
        return print_usage();
      case 'D':
        opts.print_config = true;
        break;
      case 'V':
        return version_zmq();
      case 'r':
        opts.router_endpoint = optarg;
        break;
      case 't':
        opts.connect_timeout = atoi(optarg);
        if (opts.connect_timeout < 1) {
          fprintf(stderr,
                  "error: invalid value `--connect-timeout=%d`\n",
                  opts.connect_timeout);
          return EXIT_FAILURE;
        }
        break;
      case 's':
        opts.print_serialized = true;
        break;
      case 'q':
        opts.be_terse = true;
        break;
    }
  }

  argc -= optind;
  argv += optind;

  if (opts.print_config)
    return print_config(&opts, argc, argv);

  if (argc < 1) {
    fprintf(stderr, "error: no input URI given\n");
    return EXIT_FAILURE;
  }

  jomiel_t* jomiel = jomiel_new(&opts);
  if (jomiel == NULL)
    return EXIT_FAILURE;

  int rc = EXIT_SUCCESS;
  for (c = 0; c < argc && rc == EXIT_SUCCESS; ++c)
    rc = jomiel_inquire(jomiel, argv[c]);

  jomiel_destroy(&jomiel);

  return EXIT_SUCCESS;
}

// vim: set ts=2 sw=2 tw=72 expandtab:
