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

#include <cstdio>
#include <sstream>
#include <string>

#include "jomiel/protobuf/v1beta1/status.pb.h"

namespace jomiel {

jomiel::jomiel(opts_t const& opts) : opts(opts) {
  this->zmq.endpoint = this->opts.at("--router-endpoint").asString();
  this->zmq.timeout = this->opts.at("--connect-timeout").asLong();

  this->zmq.ctx = std::make_unique<zmq::context_t>(1);
  this->zmq.sck =
    std::make_unique<zmq::socket_t>(*this->zmq.ctx, ZMQ_REQ);

  this->zmq.sck->set(zmq::sockopt::linger, 0);
}

void jomiel::connect() const {
  std::ostringstream stream;

  stream << "<connect> " << this->zmq.endpoint
         << " (timeout=" << this->zmq.timeout << ")\n";

  this->print_status(stream.str());
  this->zmq.sck->connect(this->zmq.endpoint);
}

void jomiel::inquire(std::string const& uri) const {
  this->send(uri);
  this->recv();
}

void jomiel::send(std::string const& uri) const {
  jp::Inquiry inquiry;
  inquiry.mutable_media()->set_input_uri(uri);

  if (!this->opts.at("--be-terse").asBool()) {
    this->print_message("<send>", inquiry);
  }

  std::string result;
  inquiry.SerializeToString(&result);

  this->zmq.sck->send(zmq::buffer(result));
}

void jomiel::recv() const {
  zmq::pollitem_t items[] = {
    {*this->zmq.sck, 0, ZMQ_POLLIN, 0}
  };
  zmq::message_t msg;

  if (zmq::poll(&items[0], 1, this->zmq.timeout * 1000))
    this->zmq.sck->recv(&msg);
  else
    throw std::runtime_error("connection timed out");

  auto const ptr = msg.data();
  auto const src = static_cast<char*>(ptr);

  auto const& size = msg.size();
  const std::string result(src, size);

  jp::Response resp;
  resp.ParseFromString(result);

  this->dump_response(resp);
}

void jomiel::cleanup() const {
  /*
   * "Also notice the call to ShutdownProtobufLibrary() at the end of
   * the program. All this does is delete any global objects that were
   * allocated by the Protocol Buffer library. This is unnecessary for
   * most programs, since the process is just going to exit anyway and
   * the OS will take care of reclaiming all of its memory. However,
   * if you use a memory leak checker that requires that every last
   * object be freed, or if you are writing a library which may be
   * loaded and unloaded multiple times by a single process, then you
   * may want to force Protocol Buffers to clean up everything."
   *  -- https://developers.google.com/protocol-buffers/docs/cpptutorial
   */
  gp::ShutdownProtobufLibrary();
}

void jomiel::print_status(std::string const& status) const {
  if (!this->opts.at("--be-terse").asBool()) {
    std::clog << "status: " << status;
  }
}

void jomiel::print_message(std::string const& status,
                           gp::Message const& msg) const {
  std::string result;

  if (this->opts.at("--output-json").asBool()) {
    gp::util::JsonPrintOptions opts;
    opts.add_whitespace = true;

    MessageToJsonString(msg, &result, opts);

  } else {
    gp::TextFormat::PrintToString(msg, &result);
  }

  this->print_status(status);
  std::cout << "\n" << result;
}

void jomiel::dump_terse_response(
  jp::MediaResponse const& media_response) const {
  std::cout << "---\ntitle: " << media_response.title() << "\n";
  std::cout << "quality:\n";

  for (auto const& stream : media_response.stream()) {
    auto const& quality = stream.quality();
    std::ostringstream format;
    format << "  profile: " << quality.profile() << "\n"
           << "    width: " << quality.width() << "\n"
           << "    height: " << quality.height() << "\n";
    std::cout << format.str();
  }
}

void jomiel::dump_response(jp::Response const& response) const {
  auto const& response_status = response.status();
  auto const& media_response = response.media();

  if (response_status.code() == jp::STATUS_CODE_OK) {
    if (this->opts.at("--be-terse").asBool()) {
      this->dump_terse_response(media_response);
    } else {
      this->print_message("<recv>", media_response);
    }
  } else {
    this->print_message("<recv>", response);
  }
}

}  // namespace jomiel

// vim: set ts=2 sw=2 tw=72 expandtab:
