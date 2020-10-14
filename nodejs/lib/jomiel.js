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

"use strict";

const proto = require("lib/compiled").jomiel.protobuf.v1alpha1;
const zmq = require("zeromq");

class Jomiel {
  constructor(options) {
    this.sock = new zmq.Request();
    this.options = options;
  }

  connect() {
    const re = this.options.routerEndpoint;
    this.printStatus(`<connect> ${re}`);
    this.sock.connect(re);
  }

  async inquire(uri) {
    const inquiry = proto.Inquiry.create({
      media: {
        inputUri: uri
      }
    });

    if (!this.options.beTerse) this.printMessage(`<send>`, inquiry);

    const serialized = proto.Inquiry.encode(inquiry).finish();

    this.sock.send(serialized);
    this.recv();
  }

  recv() {
    this.sock.on("message", data => {
      this.dumpResponse(data);
      this.sock.close();
    });
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

  dumpResponse(data) {
    const response = proto.Response.decode(data);
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
