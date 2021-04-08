/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

const main = () => {
  const opts = require("./options").parse();
  if (opts["--print-config"]) {
    dump_config(opts);
  } else if (opts["--version-zmq"]) {
    print_zmq_version();
  } else {
    const { Jomiel } = require("./jomiel");
    new Jomiel(opts).inquire();
  }
};

const print_zmq_version = () => {
  const zmq = require("zeromq");
  console.log("ZeroMQ version %s", zmq.version);
};

const dump_config = opts => {
  values = {};
  Object.entries(opts).forEach(([key, value]) => {
    key = key.replace("--", "");
    values[key] = value;
  });
  const result = require("js-yaml").dump(values);
  process.stdout.write(`---\n${result}`);
};

module.exports = { main };
