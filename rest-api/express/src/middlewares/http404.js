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
  res.status(404).send({
    message: `move along now, ${req.ip}. nothing to see here (${req.originalUrl}).`,
  });
