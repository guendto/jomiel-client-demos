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

import { config } from "dotenv";

config();

const { NODE_ENV, PORT, JOMIEL_ENDPOINT, JOMIEL_TIMEOUT } = process.env;

export default {
  NODE_ENV,
  PORT: PORT || 3001,
  JOMIEL_ENDPOINT: JOMIEL_ENDPOINT || "tcp://localhost:5514",
  JOMIEL_TIMEOUT: JOMIEL_TIMEOUT || 30,
};
