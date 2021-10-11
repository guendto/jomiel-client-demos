/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2019-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* eslint-disable no-console */

import jomielMessages from "jomiel-messages";
import { Request } from "zeromq";

const { Inquiry, Response, StatusCode } =
  jomielMessages.jomiel.protobuf.v1beta1;

class Jomiel {
  #opts;

  #sck;

  constructor(options) {
    this.#opts = options;
  }

  async inquire() {
    if (this.#opts.URI.length > 0) {
      try {
        this.#connect();
        for (const uri of this.#opts.URI) {
          await this.#sendInquiry(uri);
          await this.#receiveResponse();
        }
      } catch (e) {
        console.error(
          e.errno === 11 && e.code === "EAGAIN"
            ? "error: connection timed out"
            : e.stack || String(e)
        );
        process.exit(1);
      }
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
    this.#sck = new Request({ receiveTimeout: to * 1000, linger: 0 });
    this.#sck.connect(re);
  }

  async #sendInquiry(uri) {
    const msg = Inquiry.create({ media: { inputUri: uri } });

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
    if (msg.status.code === StatusCode.STATUS_CODE_OK) {
      if (this.#opts["--be-terse"]) {
        this.#dumpTerseResponse(msg.media);
      } else {
        this.#printMessage(status, msg.media);
      }
    } else {
      this.#printMessage(status, msg);
    }
  }

  // eslint-disable-next-line class-methods-use-this
  #dumpTerseResponse(msg) {
    console.log(`---\ntitle: ${msg.title}`);
    msg.stream.forEach((stream) => {
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
      console.log(this.#toJSON(msg));
    } else {
      console.log(msg);
    }
  }

  #toJSON(msg) {
    const indentation = this.#opts["--compact-json"] ? 0 : 2;
    return JSON.stringify(msg, null, indentation);
  }

  #printStatus(status) {
    if (!this.#opts["--be-terse"]) {
      console.warn(`status: ${status}`);
    }
  }
}

// factory function for Jomiel
export default (options) => new Jomiel(options);
