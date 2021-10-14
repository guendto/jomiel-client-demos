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

export const throwErrorWithStatus = (status, message) => {
  const error = new Error(message);
  error.status = status;
  throw error;
};

export default { throwErrorWithStatus };
