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

#include <docopt/docopt.h>
#include <google/protobuf/message.h>

#include <memory>
#include <string>
#include <zmq.hpp>

#include "jomiel/protobuf/v1beta1/message.pb.h"

namespace jp = jomiel::protobuf::v1beta1;
namespace gp = google::protobuf;

namespace jomiel {

using opts_t = std::map<std::string, docopt::value>;

struct jomiel {
  inline virtual ~jomiel() { cleanup(); }
  explicit jomiel(opts_t const &);

  void inquire(std::string const &) const;
  void connect() const;

private:
  void print_message(std::string const &, gp::Message const &) const;
  void print_status(std::string const &) const;

  void dump_terse_response(jp::MediaResponse const &) const;
  void dump_response(jp::Response const &) const;

  void send_inquiry(std::string const &) const;
  void receive_response() const;

  void cleanup() const;

private:
  using zctx_t = std::unique_ptr<zmq::context_t>;
  using zsck_t = std::unique_ptr<zmq::socket_t>;
  struct {
    zctx_t ctx;
    zsck_t sck;
  } zmq;
  opts_t opts;
};

} // namespace jomiel

// vim: set ts=2 sw=2 tw=72 expandtab:
