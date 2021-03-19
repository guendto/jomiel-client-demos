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

static void dump_config(jomiel::opts_t const &opts) {
  std::cout << "---\n";
  for (auto const &opt : opts) {
    auto const &name = (opt.first.find("--") != std::string::npos)
                           ? opt.first.substr(2)
                           : opt.first;
    std::cout << name << ": " << opt.second << "\n";
  }
}

static void print_zmq_version() {
  int major, minor, patch;
  std::tie(major, minor, patch) = zmq::version();
  std::cout << "ZeroMQ version " << major << "." << minor << "."
            << patch << "\n";
}

extern const char *usage;

struct runner {
  inline int run(int const argc, char const **argv) {

    auto const &opts =
        docopt::docopt(usage, {argv + 1, argv + argc}, true, "demo");

    if (opts.at("--print-config").asBool())
      dump_config(opts);
    else if (opts.at("--version-zmq").asBool())
      print_zmq_version();
    else
      jomiel::jomiel(opts).inquire();

    return EXIT_SUCCESS;
  }
};

static void inline print_error(std::exception const &error) {
  std::clog << "error: " << error.what() << "\n";
}

int main(const int argc, const char **argv) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  int r = EXIT_FAILURE;
  try {
    r = runner().run(argc, argv);
  } catch (zmq::error_t const &error) {
    print_error(error);
  } catch (std::runtime_error const &error) {
    print_error(error);
  }
  return r;
}

// vim: set ts=2 sw=2 tw=72 expandtab:
