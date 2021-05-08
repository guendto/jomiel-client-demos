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

const { InquiryHandler } = require("../handler");
const inquiryHandler = new InquiryHandler();
const router = require("express").Router();

// POST /inquiry.
router.post("/", async (req, res) => {
  inquiryHandler.handle(req, res);
});

// GET /inquiry/tube/:id
router.get("/tube/:id([0-9A-Za-z_-]{11})", async (req, res) => {
  req.body.url = `https://youtu.be/${req.params.id}`;
  inquiryHandler.handle(req, res);
});

module.exports = router;
