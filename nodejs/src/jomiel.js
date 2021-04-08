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

const { Request } = require("zeromq");

const {
  Inquiry,
  Response,
  StatusCode
} = require("./messages").jomiel.protobuf.v1beta1;

class Jomiel {
  #opts;
  #sck;

  constructor(options) {
    this.#sck = new Request();
    this.#opts = options;

    this.#sck.receiveTimeout = options["--connect-timeout"] * 1000;
  }

  inquire() {
    if (this.#opts["URI"].length > 0) {
      this.#connect();
      this.#opts["URI"].forEach(async uri => {
        try {
          await this.#sendInquiry(uri);
          await this.#receiveResponse();
        } catch (err) {
          if (err.errno == 11 && err.code == "EAGAIN") {
            console.warn("error: connection timed out");
            process.exit(1);
          } else {
            console.log(err.stack || String(err));
            throw new Error(err);
          }
        }
      });
    } else {
      console.error("error: input URI not given");
      process.exit(1);
    }
  }

  // private

  #connect() {
    const re = this.#opts["--router-endpoint"];
    const to = this.#opts["--connect-timeout"];
    this.#printStatus(`<connect> ${re} (timeout=${to})`);
    this.#sck.connect(re);
  }

  async #sendInquiry(uri) {
    const msg = Inquiry.create({
      media: { inputUri: uri }
    });

    if (!this.#opts["--be-terse"]) {
      this.#printMessage("<send>", msg);
    }

    const bytes = Inquiry.encode(msg).finish();
    await this.#sck.send(bytes);
  }

  async #receiveResponse() {
    const [bytes] = await this.#sck.receive();
    const msg = Response.decode(bytes);
    this.#dumpResponse(msg);
  }

  #dumpResponse(msg) {
    const status = "<recv>";
    if (msg.status.code == StatusCode.STATUS_CODE_OK) {
      if (this.#opts["--be-terse"]) {
        this.#dumpTerseResponse(msg.media);
      } else {
        this.#printMessage(status, msg.media);
      }
    } else {
      this.#printMessage(status, msg);
    }
  }

  #dumpTerseResponse(msg) {
    console.log(`---\ntitle: ${msg.title}`);
    msg.stream.forEach(stream => {
      const qty = stream.quality;
      console.log(
        `  profile: ${qty.profile}\n` +
          `    width: ${qty.width}\n` +
          `    height: ${qty.height}`
      );
    });
  }

  #printMessage(status, msg) {
    this.#printStatus(status);
    if (this.#opts["--output-json"]) {
      console.log(this.#to_json(msg));
    } else {
      console.log(msg);
    }
  }

  #to_json(msg) {
    const indentation = this.#opts["--compact-json"] ? 0 : 2;
    return JSON.stringify(msg, null, indentation);
  }

  #printStatus(status) {
    if (!this.#opts["--be-terse"]) {
      console.warn(`status: ${status}`);
    }
  }
}

module.exports = { Jomiel };
