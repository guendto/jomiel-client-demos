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

/* eslint-disable no-console */
/* eslint-disable import/extensions */

import config from "./helpers/config.js";
import app from "./app.js";

(() => {
  const server = app.listen(config.PORT, () => {
    console.log(`<listen> at http://localhost:${config.PORT}`);
  });

  const cleanUp = () => {
    server.close(() => {
      console.log("<exit> process gracefully");
    });
  };

  process.on("SIGINT", () => process.kill(process.pid, "SIGTERM"));
  process.on("SIGTERM", () => cleanUp());
})();
