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
import "express-async-errors";

/* eslint-disable import/extensions */
import jomiel from "../jomiel.js";

const router = Router();

// GET /api/yt/by-id/:id
router.get("/:id([0-9A-Za-z_-]{11})", async (req, res) => {
  req.body.inputUri = `https://youtu.be/${req.params.id}`;
  await jomiel.sendInquiry(res, req.body);
});

export default router;
