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

import jomielMessages from "jomiel-messages";
import { Jomiel } from "./jomiel.js";

const { StatusCode } = jomielMessages.jomiel.protobuf.v1beta1;

export class InquiryHandler {
  #jomiel = new Jomiel();

  async handle(req, res) {
    try {
      const msg = await this.#jomiel.inquire(req.body.url);
      this.#sendResponse(msg, res);
    } catch (error) {
      this.#jomiel.handleError(error, res);
    }
  }

  #sendResponse(jomielResponse, expressResponse) {
    const send = (msg, res, statusCode, statusMessage) => {
      res.status(statusCode).send({
        status: statusMessage,
        data: msg
      });
    };

    const success = (msg, res) => {
      send(msg, res, 200, "ok");
    };

    const failure = (msg, res) => {
      send(msg, res, msg.status.code, "failed");
    };

    jomielResponse.status.code == StatusCode.STATUS_CODE_OK
      ? success(jomielResponse.media, expressResponse)
      : failure(jomielResponse, expressResponse);
  }
}
