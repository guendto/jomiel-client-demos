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

import express from "express";

/* eslint-disable import/extensions */

import inquiryRouter from "./controllers/inquiry.js";
import errorHandler from "./middlewares/error.js";
import http404 from "./middlewares/http404.js";
import config from "./config.js";

(() => {
  const app = express();

  app.use(express.json());
  app.use("/inquiry", inquiryRouter);
  app.use(errorHandler);
  app.use(http404);

  const server = app.listen(config.PORT, () => {
    console.log(`<listen> at http://localhost:${config.PORT}`);
    inquiryRouter.stack.forEach((layer) => {
      const { route } = layer;
      const { method } = route.stack[0];
      console.log(`<endpoint> ${method} /inquiry${route.path}`);
    });
  });

  const cleanUp = () => {
    server.close(() => {
      console.log("<exit> process gracefully");
    });
  };

  process.on("SIGINT", () => process.kill(process.pid, "SIGTERM"));
  process.on("SIGTERM", () => cleanUp());
})();
