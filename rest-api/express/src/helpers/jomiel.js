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

import messages from "jomiel-messages";
import { Request } from "zeromq";

/* eslint-disable import/extensions */
import { throwErrorWithStatus } from "./error.js";
import config from "./config.js";

const { STATUS_CODE_OK } = messages.jomiel.protobuf.v1beta1.StatusCode;
const { Inquiry, Response } = messages.jomiel.protobuf.v1beta1;

const connect = () => {
  const socket = new Request({
    receiveTimeout: config.JOMIEL_TIMEOUT * 1000,
    linger: 0,
  });

  socket.connect(config.JOMIEL_ENDPOINT);

  if (config.NODE_ENV !== "test") {
    const configuration = `${config.JOMIEL_ENDPOINT}, timeout=${config.JOMIEL_TIMEOUT}`;
    // eslint-disable-next-line no-console
    console.log(`<jomiel> connect a socket (${configuration})`);
  }

  return socket;
};

const socket = connect();

const receiveResponse = async () => {
  const [bytes] = await socket.receive();
  return Response.decode(bytes);
};

// eslint-disable-next-line consistent-return
const sendInquiry = async (res, { inputUri }) => {
  const message = Inquiry.create({ media: { inputUri } });
  const bytes = Inquiry.encode(message).finish();

  socket.send(bytes);
  const response = await receiveResponse();

  if (response.status.code === STATUS_CODE_OK) {
    return res.json({ status: "ok", data: response.media });
  }
  throwErrorWithStatus(500, response.status.message);
};

export default { sendInquiry };
