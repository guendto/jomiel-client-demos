/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */
import { dump as dumpYAML } from "js-yaml";
import { version } from "zeromq";

import { Jomiel } from "./jomiel.js";
import { parse } from "./options.js";

const dump_config = opts => {
  let values = {};
  Object.entries(opts).forEach(([key, value]) => {
    key = key.replace("--", "");
    values[key] = value;
  });
  process.stdout.write(`---\n${dumpYAML(values)}`);
};

(() => {
  const opts = parse();
  if (opts["--print-config"]) {
    dump_config(opts);
  } else if (opts["--version-zmq"]) {
    console.log("ZeroMQ version %s", version);
  } else {
    new Jomiel(opts).inquire();
  }
})();
