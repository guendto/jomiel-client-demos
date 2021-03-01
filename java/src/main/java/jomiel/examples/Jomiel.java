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

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.Message;
import com.google.protobuf.util.JsonFormat;
import java.io.IOException;
import java.util.List;
import jomiel.protobuf.v1beta1.Inquiry;
import jomiel.protobuf.v1beta1.MediaInquiry;
import jomiel.protobuf.v1beta1.MediaResponse;
import jomiel.protobuf.v1beta1.MediaResponse.Stream;
import jomiel.protobuf.v1beta1.MediaResponse.Stream.StreamQuality;
import jomiel.protobuf.v1beta1.Response;
import jomiel.protobuf.v1beta1.ResponseStatus;
import jomiel.protobuf.v1beta1.StatusCode;
import org.zeromq.SocketType;
import org.zeromq.ZContext;
import org.zeromq.ZMQ.Poller;
import org.zeromq.ZMQ.Socket;

@SuppressWarnings("PMD.BeanMembersShouldSerialize")
public final class Jomiel {
  private List<String> input = null;
  private Options options = null;

  public Jomiel(final Options options) throws IOException {
    this.options = options;
    this.input = options.getUri();
    if (this.input.isEmpty()) {
      throw new IOException("no input URI given");
    }
  }

  public static Jomiel create(final Options options) throws IOException {
    return new Jomiel(options);
  }

  public void inquire() throws InvalidProtocolBufferException {
    try (ZContext ctx = new ZContext();
        Socket sck = ctx.createSocket(SocketType.REQ);
        Poller poller = ctx.createPoller(1)) {

      sck.setLinger(0);
      connect(sck);
      poller.register(sck, Poller.POLLIN);

      for (final String uri : input) {
        send(sck, uri);
        recv(sck, poller);
      }

    } catch (final Exception exc) {
      throw exc;
    }
  }

  private String getConnectMessage() {
    return String.format(
        "<connect> %s (timeout=%d)",
        options.getRouterEndpoint(), options.getConnectTimeout() / 1000);
  }

  private void connect(final Socket sck) {
    printStatus(getConnectMessage());
    sck.connect(options.getRouterEndpoint());
  }

  private void send(final Socket sck, final String uri) throws InvalidProtocolBufferException {
    final MediaInquiry mediaInquiry = MediaInquiry.newBuilder().setInputUri(uri).build();
    final Inquiry inquiry = Inquiry.newBuilder().setMedia(mediaInquiry).build();
    final byte[] data = inquiry.toByteArray();
    sck.send(data);
    if (!options.beTerse()) {
      printMessage("<send>", inquiry);
    }
  }

  private void recv(final Socket sck, final Poller poller) throws InvalidProtocolBufferException {
    poller.poll(options.getConnectTimeout());
    if (poller.pollin(0)) {
      final byte[] data = sck.recv();
      final Response response = Response.parseFrom(data);
      dumpResponse(response);
    } else {
      throw new RuntimeException("connection timed out");
    }
  }

  private void printStatus(final String message) {
    if (!options.beTerse()) {
      System.err.println("status: " + message);
    }
  }

  private void dumpResponse(final Response response) throws InvalidProtocolBufferException {

    final ResponseStatus responseStatus = response.getStatus();
    final MediaResponse mediaResponse = response.getMedia();

    if (responseStatus.getCode() == StatusCode.STATUS_CODE_OK) {
      if (options.beTerse()) {
        dumpTerseResponse(mediaResponse);
      } else {
        printMessage("<recv>", mediaResponse);
      }
    } else {
      printMessage("<recv>", response);
    }
  }

  private String getTerseQualityString(final StreamQuality quality) {
    return String.format(
        "  profile: %s%n    width: %d%n    height: %d",
        quality.getProfile(), quality.getWidth(), quality.getHeight());
  }

  private void dumpTerseResponse(final MediaResponse mediaResponse) {

    System.out.println("---\ntitle: " + mediaResponse.getTitle());
    System.out.println("quality:");

    for (final Stream stream : mediaResponse.getStreamList()) {
      final StreamQuality streamQuality = stream.getQuality();
      final String qualityString = getTerseQualityString(streamQuality);
      System.out.println(qualityString);
    }
  }

  private void printMessage(final String status, final Message msg)
      throws InvalidProtocolBufferException {

    String result;

    if (options.outputAsJson()) {

      final JsonFormat.Printer printer =
          JsonFormat.printer()
              // .omittingInsignificantWhitespace()
              .preservingProtoFieldNames();

      result = printer.print(msg) + "\n";

    } else {
      result = msg.toString();
    }

    printStatus(status);
    System.out.print(result);
  }
}
