/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2019-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */
import { dump as dumpYAML } from "js-yaml";
import { version } from "zeromq";

/* eslint-disable import/extensions */
import Jomiel from "./jomiel.js";
import parse from "./options.js";

const dumpConfig = (opts) => {
  const values = {};
  Object.entries(opts).forEach(([key, value]) => {
    // eslint-disable-next-line no-param-reassign
    key = key.replace("--", "");
    values[key] = value;
  });
  process.stdout.write(`---\n${dumpYAML(values)}`);
};

(() => {
  const opts = parse();
  if (opts["--print-config"]) {
    dumpConfig(opts);
  } else if (opts["--version-zmq"]) {
    // eslint-disable-next-line no-console
    console.log("ZeroMQ version %s", version);
  } else {
    Jomiel(opts).inquire();
  }
})();
