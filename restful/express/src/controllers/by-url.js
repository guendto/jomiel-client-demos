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

/* eslint-disable import/extensions */

import { Router } from "express";
import "express-async-errors";

import { throwErrorWithStatus } from "../helpers/error.js";
import jomiel from "../helpers/jomiel.js";

const router = Router();

// POST /api/by-url
router.post("/", async (req, res) => {
  const { inputUri } = req.body;
  if (!inputUri) {
    throwErrorWithStatus(400, "missing inputUri");
  }
  await jomiel.sendInquiry(res, req.body);
});

export default router;
