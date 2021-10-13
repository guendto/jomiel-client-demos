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

import express from "express";
import morgan from "morgan";
import compression from "compression";
import helmet from "helmet";

/* eslint-disable import/extensions */

import byUrlRouter from "./controllers/by-url.js";
import ytRouter from "./controllers/yt.js";
import errorHandler from "./middlewares/error.js";
import http404 from "./middlewares/http404.js";
import config from "./helpers/config.js";

const app = express();

app.use(express.json());
if (config.NODE_ENV !== "test") {
  app.use(morgan("dev"));
}
app.use(compression());
app.use(helmet());
app.use("/api/by-url", byUrlRouter);
app.use("/api/yt/by-id", ytRouter);
app.use(errorHandler);
app.use(http404);

export default app;
