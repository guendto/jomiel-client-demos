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

/* eslint-disable no-console */
/* eslint-disable import/extensions */

import config from "./helpers/config.js";
import jomiel from "./helpers/jomiel.js";
import app from "./app.js";

(() => {
  jomiel.connect();

  const server = app.listen(config.PORT, () => {
    console.log(`<listen> on http://localhost:${config.PORT}`);
  });

  const cleanUp = () => {
    server.close(() => {
      console.log("<exit> process gracefully");
      jomiel.disconnect();
    });
  };

  process.on("SIGINT", () => process.kill(process.pid, "SIGTERM"));
  process.on("SIGTERM", () => cleanUp());
})();
