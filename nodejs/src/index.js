/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019-2020 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

"use strict";

/*
process.env.NODE_PATH = ".";
require("module").Module._initPaths();
*/

const opts = require("./options").parse();

if (opts.versionZmq) {
  const zmq = require("zeromq");
  console.log("ZeroMQ version %s", zmq.version);
  process.exit();
}

if (!opts.args.length) {
  console.error("error: input URI not given");
  process.exit(1);
}

const { Jomiel } = require("./jomiel");
const jomiel = Jomiel.create(opts);

jomiel.connect();

opts.args.forEach(async (uri) => {
  try {
    await jomiel.inquire(uri);
  } catch (e) {
    if (e.errno == 11 && e.code == "EAGAIN") {
      console.error("error: connection timed out");
      process.exit(1);
    } else {
      throw new Error(e);
    }
  }
});

// vim: set ts=2 sw=2 tw=72 expandtab:
