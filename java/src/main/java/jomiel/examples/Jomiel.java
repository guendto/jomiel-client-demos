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

package jomiel.examples;

import static com.google.protobuf.util.JsonFormat.printer;
import static java.lang.String.format;
import static java.lang.System.exit;
import static java.util.Collections.unmodifiableList;
import static jomiel.protobuf.v1beta1.StatusCode.STATUS_CODE_OK;
import static org.tinylog.Logger.error;
import static org.tinylog.Logger.info;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.Message;
import com.google.protobuf.util.JsonFormat.Printer;
import jomiel.protobuf.v1beta1.Inquiry;
import jomiel.protobuf.v1beta1.MediaInquiry;
import jomiel.protobuf.v1beta1.MediaResponse;
import jomiel.protobuf.v1beta1.Response;
import org.zeromq.SocketType;
import org.zeromq.ZContext;
import org.zeromq.ZMQ.Poller;
import org.zeromq.ZMQ.Socket;

@SuppressWarnings({
  "PMD.BeanMembersShouldSerialize",
  "PMD.DataflowAnomalyAnalysis",
  "PMD.DoNotCallSystemExit"
})
final class Jomiel {
  private final Options opts;
  private final ZContext ctx = new ZContext();
  private final Socket sck = ctx.createSocket(SocketType.REQ);
  private final Poller poller = ctx.createPoller(1);
  private Printer jsonFormatter = printer();

  Jomiel(final Options options) {
    opts = options;
    sck.setLinger(0);
    poller.register(sck, Poller.POLLIN);
    if (opts.compactJson) {
      jsonFormatter = printer().omittingInsignificantWhitespace();
    }
  }

  void run() throws InvalidProtocolBufferException {
    if (opts.uri.isEmpty()) {
      error("error: input URI not given");
      exit(1);
    }
    connect();
    for (final var uri : unmodifiableList(opts.uri)) {
      inquire(uri);
    }
  }

  private void connect() {
    final var re = opts.routerEndpoint;
    final var to = opts.connectTimeout;
    printStatus(format("<connect> %s (timeout=%d)", re, to));
    sck.connect(re);
  }

  private void inquire(final String uri) throws InvalidProtocolBufferException {
    sendInquiry(uri);
    receiveResponse();
  }

  private void sendInquiry(final String uri) throws InvalidProtocolBufferException {
    final var mnq = MediaInquiry.newBuilder().setInputUri(uri).build();
    final var msg = Inquiry.newBuilder().setMedia(mnq).build();
    if (!opts.beTerse) {
      printMessage("<send>", msg);
    }
    sck.send(msg.toByteArray());
  }

  private void receiveResponse() throws InvalidProtocolBufferException {
    poller.poll(opts.connectTimeout * 1_000);
    if (poller.pollin(0)) {
      final var bytes = sck.recv();
      final var msg = Response.parseFrom(bytes);
      dumpResponse(msg);
    } else {
      error("error: connection timed out");
      exit(1);
    }
  }

  private void printStatus(final String status) {
    if (!opts.beTerse) {
      error("status: " + status);
    }
  }

  private void dumpResponse(final Response msg) throws InvalidProtocolBufferException {
    final var media = msg.getMedia();
    final var status = "<recv>";
    if (msg.getStatus().getCode() == STATUS_CODE_OK) {
      if (opts.beTerse) {
        dumpTerseResponse(media);
      } else {
        printMessage(status, media);
      }
    } else {
      printMessage(status, msg);
    }
  }

  private void dumpTerseResponse(final MediaResponse msg) {
    info(format("---%ntitle: %s%nquality:", msg.getTitle()));
    for (final var stream : unmodifiableList(msg.getStreamList())) {
      final var quality = stream.getQuality();
      final String str;
      str =
          format(
              "  profile: %s%n    width: %d%n    height: %d",
              quality.getProfile(), quality.getWidth(), quality.getHeight());
      info(str);
    }
  }

  private void printMessage(final String status, final Message msg)
      throws InvalidProtocolBufferException {
    final String result;
    result = (opts.outputJson) ? jsonFormatter.print(msg) : msg.toString();
    printStatus(status);
    info(result);
  }
}
