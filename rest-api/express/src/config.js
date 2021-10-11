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

import { config } from "dotenv";

config();

const { PORT } = process.env;

export default { PORT: PORT || 3001 };
