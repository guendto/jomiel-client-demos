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

require("dotenv").config();

const inquiryRouter = require("./routes/inquiry");
const express = require("express");
const app = express();

app.use(express.json());
app.use("/inquiry", inquiryRouter);

app.get("*", (req, res, next) => {
  const error = new Error(
    `${req.ip} tried to access ${req.originalUrl}`
  );
  error.StatusCode = 301;
  next(error);
});

app.use((error, req, res, next) => {
  return res.status(500).json({ status: error.toString() });
});

app.listen(process.env.PORT || 3001, () => {
  console.log(`<listen> at http://localhost:${process.env.PORT}`);
  inquiryRouter.stack.forEach(layer => {
    const route = layer.route;
    const method = route.stack[0].method;
    console.log(`<endpoint> ${method} /inquiry${route.path}`);
  });
});
