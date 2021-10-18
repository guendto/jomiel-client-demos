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

export default (req, res) =>
  res.status(404).json({
    status: "error",
    data: {
      message: `not found (${req.originalUrl})`,
    },
  });
