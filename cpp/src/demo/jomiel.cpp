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

#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>

#include <chrono>
#include <cstdio>
#include <sstream>
#include <string>

#include "jomiel/protobuf/v1beta1/status.pb.h"

namespace jomiel {

jomiel::jomiel(opts_t const &opts) : opts(opts) {
  zmq.ctx = std::make_unique<zmq::context_t>(1);
  zmq.sck = std::make_unique<zmq::socket_t>(*zmq.ctx, ZMQ_REQ);
  compat_zmq_set_options();
}

void jomiel::inquire() const {
  auto const &items = opts.at("URI").asStringList();
  if (!items.empty()) {
    connect();
    for (auto const &uri : items) {
      send_inquiry(uri);
      receive_response();
    }
  } else
    throw std::runtime_error("input URI not given");
}

void jomiel::connect() const {
  auto const &re = opts.at("--router-endpoint").asString();
  auto const &to = opts.at("--connect-timeout").asLong();

  std::ostringstream stream;
  stream << "<connect> " << re << " (timeout=" << to << ")";

  print_status(stream.str());
  zmq.sck->connect(re);
}

void jomiel::send_inquiry(std::string const &uri) const {
  jp::Inquiry inquiry;
  inquiry.mutable_media()->set_input_uri(uri);

  if (!opts.at("--be-terse").asBool())
    print_message("<send>", inquiry);

  std::string msg;
  inquiry.SerializeToString(&msg);
  compat_zmq_send(msg);
}

void jomiel::receive_response() const {
  zitems_t items;
  compat_zmq_pollitems(items);

  auto const &to = opts.at("--connect-timeout").asLong() * 1000;

  zmq::message_t bytes;
  if (zmq::poll(items, std::chrono::milliseconds(to)))
    compat_zmq_read(bytes);
  else
    throw std::runtime_error("connection timed out");

  jp::Response msg;
  compat_zmq_parse(msg, bytes);
  dump_response(msg);
}

void jomiel::print_status(std::string const &status) const {
  if (!opts.at("--be-terse").asBool())
    std::clog << "status: " << status << "\n";
}

void jomiel::dump_response(jp::Response const &msg) const {
  const auto &status = "<recv>";
  if (msg.status().code() == jp::STATUS_CODE_OK) {
    auto const &media = msg.media();
    if (opts.at("--be-terse").asBool()) {
      dump_terse_response(media);
    } else {
      print_message(status, media);
    }
  } else {
    print_message(status, msg);
  }
}

void jomiel::dump_terse_response(jp::MediaResponse const &msg) const {
  std::cout << "---\ntitle: " << msg.title() << "\nquality:\n";
  for (auto const &stream : msg.stream()) {
    std::cout << "  profile: " << stream.quality().profile() << "\n"
              << "    width: " << stream.quality().width() << "\n"
              << "    height: " << stream.quality().height() << "\n";
  }
}

void jomiel::print_message(std::string const &status,
                           gp::Message const &msg) const {
  print_status(status);

  std::string result;
  if (opts.at("--output-json").asBool())
    to_json(msg, result);
  else
    gp::TextFormat::PrintToString(msg, &result);

  std::cout << result;
}

void jomiel::to_json(gp::Message const &msg, std::string &dst) const {
  auto const &compact = opts.at("--compact-json").asBool();

  gp::util::JsonPrintOptions print_opts;
  print_opts.add_whitespace = !compact;

  MessageToJsonString(msg, &dst, print_opts);

  if (compact)
    dst += "\n";
}

void jomiel::cleanup() const {
  /*
   * "Also notice the call to ShutdownProtobufLibrary() at the end of
   * the program. All this does is delete any global objects that were
   * allocated by the Protocol Buffer library. This is unnecessary for
   * most programs, since the process is just going to exit anyway and
   * the OS will take care of reclaiming all of its memory. However, if
   * you use a memory leak checker that requires that every last object
   * be freed, or if you are writing a library which may be loaded and
   * unloaded multiple times by a single process, then you may want to
   * force Protocol Buffers to clean up everything."
   *  <https://developers.google.com/protocol-buffers/docs/cpptutorial>
   */
  gp::ShutdownProtobufLibrary();
}

// cppzmq compatibility functions.
//

void jomiel::compat_zmq_set_options() const {
#if CPPZMQ_VERSION >= ZMQ_MAKE_VERSION(4, 7, 0)
  zmq.sck->set(zmq::sockopt::linger, 0);
#else
  int n = 0;
  zmq.sck->setsockopt(ZMQ_LINGER, &n, sizeof(n));
#endif
}

void jomiel::compat_zmq_send(std::string const &msg) const {
#if CPPZMQ_VERSION >= ZMQ_MAKE_VERSION(4, 4, 0)
  zmq.sck->send(zmq::buffer(msg));
#else
  zmq.sck->send(msg.c_str(), msg.size());
#endif
}

void jomiel::compat_zmq_read(zmq::message_t &bytes) const {
#if CPPZMQ_VERSION >= ZMQ_MAKE_VERSION(4, 3, 1)
  zmq.sck->recv(bytes);
#else
  zmq.sck->recv(&bytes);
#endif
}

void jomiel::compat_zmq_pollitems(zitems_t &dst) const {
#if CPPZMQ_VERSION >= ZMQ_MAKE_VERSION(4, 3, 1)
  zmq::pollitem_t item = {*zmq.sck, 0, ZMQ_POLLIN, 0};
#else
  zmq::pollitem_t const item = {static_cast<void *>(*zmq.sck), 0,
                                ZMQ_POLLIN};
#endif
  dst.push_back(item);
}

void jomiel::compat_zmq_parse(jp::Response &msg,
                              zmq::message_t const &bytes) const {
#if CPPZMQ_VERSION >= ZMQ_MAKE_VERSION(4, 6, 0)
  msg.ParseFromString(bytes.to_string());
#else
  std::string const str(static_cast<const char *>(bytes.data()),
                        bytes.size());
  msg.ParseFromString(str);
#endif
}

} // namespace jomiel

// vim: set ts=2 sw=2 tw=72 expandtab:
