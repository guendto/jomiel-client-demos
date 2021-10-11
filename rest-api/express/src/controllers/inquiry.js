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

import { Router } from "express";

// eslint-disable-next-line import/extensions
import { InquiryHandler } from "../handler.js";

const inquiryHandler = new InquiryHandler();
const router = Router();

// POST /inquiry.
router.post("/", async (req, res) => {
  inquiryHandler.handle(req, res);
});

// GET /inquiry/tube/:id
router.get("/tube/:id([0-9A-Za-z_-]{11})", async (req, res) => {
  req.body.url = `https://youtu.be/${req.params.id}`;
  inquiryHandler.handle(req, res);
});

export default router;
