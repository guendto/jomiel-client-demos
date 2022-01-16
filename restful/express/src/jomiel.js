/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2021-2022 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

import messages from "jomiel-messages";
import { Request } from "zeromq";

/* eslint-disable import/extensions */
import { throwErrorWithStatus } from "./helpers/error.js";
import config from "./helpers/config.js";

const { STATUS_CODE_OK } = messages.jomiel.protobuf.v1beta1.StatusCode;
const { Inquiry, Response } = messages.jomiel.protobuf.v1beta1;

class Jomiel {
  #socket;

  constructor() {
    this.#socket = new Request({
      receiveTimeout: config.JOMIEL_TIMEOUT * 1000,
      linger: 0,
    });
  }

  connect() {
    this.#socket.connect(config.JOMIEL_ENDPOINT);

    if (config.NODE_ENV !== "test") {
      const info = `${config.JOMIEL_ENDPOINT}, timeout=${config.JOMIEL_TIMEOUT}`;
      console.log(`<jomiel> connected (${info})`);
    }
  }

  disconnect() {
    if (config.NODE_ENV !== "test") {
      console.log("<jomiel> disconnected");
    }
    this.#socket.close();
  }

  async sendInquiry(res, { inputUri }) {
    const message = Inquiry.create({ media: { inputUri } });
    const encoded = Inquiry.encode(message).finish();
    await this.#socket.send(encoded);
    await this.#receiveResponse(res);
  }

  async #receiveResponse(res) {
    const [bytes] = await this.#socket.receive();
    const message = Response.decode(bytes);

    const { status, media: data } = message;
    if (status.code === STATUS_CODE_OK) {
      return res.json({ status: "ok", data });
    }

    throwErrorWithStatus(500, status.message);
  }
}

export default new Jomiel();
