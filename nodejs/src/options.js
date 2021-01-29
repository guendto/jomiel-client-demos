/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2020 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

"use strict";

const parse = () => {
  const cmdr = require("commander");

  cmdr
    .usage("[options] <uri ...>")
    .option(
      "-r, --router-endpoint <addr>",
      "Specify the router endpoint address",
      "tcp://localhost:5514"
    )
    .option(
      "-t, --connect-timeout <secs>",
      "Specify maximum time in seconds for the connection allowed to take",
      30
    )
    .option("-V, --version-zmq", "Display ZeroMQ version and exit")
    .option("-j, --output-json", "Print dumped messages in JSON")
    .option(
      "-q, --be-terse",
      "Be brief and to the point; dump interesting details only"
    );

  return cmdr.parse(process.argv);
};

module.exports = { parse };
