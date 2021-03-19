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

static void version_zmq() {
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

    if (opts.at("--print-config").asBool()) {
      dump_config(opts);
    } else if (opts.at("--version-zmq").asBool()) {
      version_zmq();
    } else {
      const auto &uri = opts.at("URI").asStringList();
      if (uri.empty())
        throw std::runtime_error("input URI not given");

      jomiel::jomiel const jomiel(opts);
      jomiel.connect();

      for (auto const &_uri : uri)
        jomiel.inquire(_uri);
    }
    return EXIT_SUCCESS;
  }
};

int main(const int argc, const char **argv) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  int r = EXIT_FAILURE;
  try {
    r = runner().run(argc, argv);
  } catch (zmq::error_t const &error) {
    std::cerr << "error: libzmq: " << error.what() << "\n";
  } catch (std::runtime_error const &error) {
    std::cerr << "error: " << error.what() << "\n";
  }
  return r;
}

// vim: set ts=2 sw=2 tw=72 expandtab:
