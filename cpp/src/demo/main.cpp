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

#include "demo/jomiel.h"

static void print_config(const jomiel::opts_t& opts) {
  for (auto const& opt : opts) {
    std::cout << opt.first << ": " << opt.second << "\n";
  }
  exit(EXIT_SUCCESS);
}

static void version_zmq() {
  int major, minor, patch;
  std::tie(major, minor, patch) = zmq::version();
  std::cout << "ZeroMQ version " << major << "." << minor << "." << patch
            << "\n";
  exit(EXIT_SUCCESS);
}

extern const char* usage;

struct runner {
  inline int run(int const argc, char const** argv) {
    auto const& opts =
        docopt::docopt(usage, {argv + 1, argv + argc}, true, "demo");

    if (opts.at("--print-config").asBool()) {
      print_config(opts);
    } else if (opts.at("--version-zmq").asBool()) {
      version_zmq();
    }

    jomiel::strvect_t const input_uri = opts.at("URI").asStringList();

    if (input_uri.empty())
      throw std::runtime_error("input URI not given");

    jomiel::jomiel const jomiel(opts);
    jomiel.connect();

    for (auto const& uri : input_uri)
      jomiel.inquire(uri);

    return EXIT_SUCCESS;
  }
};

int main(const int argc, const char** argv) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  int r = EXIT_FAILURE;
  try {
    r = runner().run(argc, argv);
  } catch (zmq::error_t const& error) {
    std::cerr << "error: libzmq: " << error.what() << "\n";
  } catch (std::runtime_error const& error) {
    std::cerr << "error: " << error.what() << "\n";
  }
  return r;
}

// vim: set ts=2 sw=2 tw=72 expandtab:
