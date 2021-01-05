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

#ifndef CPP_SRC_DEMO_JOMIEL_H_
#define CPP_SRC_DEMO_JOMIEL_H_

#include <google/protobuf/message.h>

#include <memory>
#include <string>
#include <zmq.hpp>

#include "demo/types.h"
#include "jomiel/protobuf/v1beta1/message.pb.h"

namespace jp = jomiel::protobuf::v1beta1;
namespace gp = google::protobuf;

namespace jomiel {

struct jomiel {
  inline virtual ~jomiel() { cleanup(); }
  explicit jomiel(opts_t const&);

  void inquire(std::string const&) const;
  void connect() const;

 private:
  void print_message(std::string const&, gp::Message const&) const;
  void print_status(std::string const&) const;

  void dump_terse_response(jp::MediaResponse const&) const;
  void dump_response(jp::Response const&) const;

  void send(std::string const&) const;
  void recv() const;

  void cleanup() const;

 private:
  using zctx_t = std::unique_ptr<zmq::context_t>;
  using zsck_t = std::unique_ptr<zmq::socket_t>;
  struct {
    std::string endpoint;
    int16_t timeout;
    zctx_t ctx;
    zsck_t sck;
  } zmq;
  opts_t opts;
};

}  // namespace jomiel

#endif  // CPP_SRC_JOMIEL_H_

// vim: set ts=2 sw=2 tw=72 expandtab:
