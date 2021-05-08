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
"use strict";

const {
  StatusCode
} = require("jomiel-messages").jomiel.protobuf.v1beta1;
const { Jomiel } = require("./jomiel");

class InquiryHandler {
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

module.exports = { InquiryHandler };
