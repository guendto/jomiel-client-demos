/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

const { Request } = require("zeromq");

const {
  Inquiry,
  Response
} = require("./messages").jomiel.protobuf.v1beta1;

class Jomiel {
  #sck;

  inquire(uri) {
    this.#connect();
    this.#sendInquiry(uri);
    return this.#receiveResponse();
  }

  handleError(error, expressResult) {
    if (error.errno == 11 && error.code == "EAGAIN") {
      return expressResult.status(500).send({
        status: "jomiel: connection timed out"
      });
    } else {
      console.log(error.stack || String(error));
      throw error;
    }
  }

  // private

  #connect() {
    const re = process.env.JOMIEL_ENDPOINT;
    const to = process.env.JOMIEL_TIMEOUT;
    console.log(`<jomiel> connected (${re}, timeout=${to})`);
    this.#sck = new Request();
    this.#sck.receiveTimeout = to * 1000; // to msec
    this.#sck.linger = 0;
    this.#sck.connect(re);
  }

  #sendInquiry(uri) {
    const msg = Inquiry.create({ media: { inputUri: uri } });
    const bytes = Inquiry.encode(msg).finish();
    this.#sck.send(bytes);
  }

  async #receiveResponse() {
    const [bytes] = await this.#sck.receive();
    return Response.decode(bytes);
  }
}

module.exports = { Jomiel };
