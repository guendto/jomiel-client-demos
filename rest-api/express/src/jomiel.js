/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

import jomielMessages from "jomiel-messages";
import { Request } from "zeromq";

const { Inquiry, Response } = jomielMessages.jomiel.protobuf.v1beta1;

export class Jomiel {
  #sck;

  inquire(uri) {
    this.#connect();
    this.#sendInquiry(uri);
    return this.#receiveResponse();
  }

  handleError(error, expressResult) {
    if (error.errno === 11 && error.code === "EAGAIN") {
      return expressResult.status(500).send({
        status: "jomiel: connection timed out",
      });
    }
    console.log(error.stack || String(error));
    throw error;
  }

  // private

  #connect() {
    const re = process.env.JOMIEL_ENDPOINT;
    const to = process.env.JOMIEL_TIMEOUT;
    console.log(`<jomiel> connected (${re}, timeout=${to})`);
    this.#sck = new Request({ receiveTimeout: to * 1000, linger: 0 });
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

// factory function for Jomiel
export default (options) => new Jomiel();
