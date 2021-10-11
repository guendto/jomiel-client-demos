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

// eslint-disable-next-line import/extensions
import Jomiel from "./jomiel.js";

const { StatusCode } = jomielMessages.jomiel.protobuf.v1beta1;

class InquiryHandler {
  #jomiel = Jomiel();

  async handle(req, res) {
    try {
      const msg = await this.#jomiel.inquire(req.body.url);
      this.sendResponse(msg, res);
    } catch (error) {
      this.#jomiel.handleError(error, res);
    }
  }

  // static

  // eslint-disable-next-line consistent-return
  static sendResponse(jomielResponse, expressResponse) {
    const send = (msg, res, statusCode, statusMessage) =>
      res.status(statusCode).send({
        status: statusMessage,
        data: msg,
      });

    const success = (msg, res) => send(msg, res, 200, "ok");

    const failed = (msg, res) =>
      send(msg, res, msg.status.code, "error");

    const { code } = jomielResponse.status;
    const { STATUS_CODE_OK } = StatusCode;

    if (code === STATUS_CODE_OK) {
      return success(jomielResponse.media, expressResponse);
    }

    failed(jomielResponse, expressResponse);
  }
}

export default InquiryHandler;
