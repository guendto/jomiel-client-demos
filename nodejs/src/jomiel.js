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

"use strict";

const proto = require("./compiled").jomiel.protobuf.v1alpha1;
const zmq = require("zeromq");

class Jomiel {
  constructor(options) {
    this.options = options;
  }

  connect() {
    this.sock = new zmq.Request();
    this.sock.receiveTimeout = this.options.connectTimeout * 1000;
    const re = this.options.routerEndpoint;
    this.printStatus(`<connect> ${re}`);
    this.sock.connect(re);
  }

  async inquire(uri) {
    const inquiry = proto.Inquiry.create({
      media: { inputUri: uri }
    });

    const serialize = msg => proto.Inquiry.encode(msg).finish();
    const deserialize = msg => proto.Response.decode(msg);

    if (!this.options.beTerse) this.printMessage(`<send>`, inquiry);

    await this.sock.send(serialize(inquiry));
    const [result] = await this.sock.receive();

    this.dumpResponse(deserialize(result));
  }

  printStatus(message) {
    if (!this.options.beTerse) {
      console.error("status: " + message);
    }
  }

  dumpTerseResponse(mediaResponse) {
    console.log(`---\ntitle: ` + mediaResponse.title);
    mediaResponse.stream.forEach(stream => {
      const quality = stream.quality;
      console.log(`  profile: ${quality.profile}`);
      console.log(`    width: ${quality.width}`);
      console.log(`    height: ${quality.height}`);
    });
  }

  dumpResponse(response) {
    if (response.status.code == proto.StatusCode.STATUS_CODE_OK) {
      if (this.options.beTerse) {
        this.dumpTerseResponse(response.media);
      } else {
        this.printMessage(`<recv>`, response.media);
      }
    } else {
      this.printMessage(`<recv>`, response);
    }
  }

  printMessage(status, message) {
    let str;
    if (this.options.outputJson) {
      // 2=prettyprint with two-space indentation
      str = JSON.stringify(message, null, 2);
    } else {
      str = message;
    }

    this.printStatus(status);
    console.log(str);
  }

  static create(options) {
    return new Jomiel(options);
  }
}

module.exports = {
  Jomiel
};

// vim: set ts=2 sw=2 tw=72 expandtab:
