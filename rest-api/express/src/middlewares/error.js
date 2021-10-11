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

// eslint-disable-next-line no-unused-vars
export default (error, req, res, next) => {
  const { status, message } = error;
  res.status(status || 500).json({ status: "error", message });
  // return next(error);
};
