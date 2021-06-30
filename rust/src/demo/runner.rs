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

use super::jomiel::Jomiel;
use super::options::Options;
use structopt::StructOpt;

pub fn main() {
    let opts = Options::from_args();
    if opts.print_config {
        dump_config(&opts);
    } else if opts.version_zmq {
        print_zmq_version();
    } else {
        Jomiel::new(opts).inquire();
    }
}

fn dump_config(opts: &Options) {
    let s = serde_yaml::to_string(opts).unwrap();
    print!("{}", s);
}

fn print_zmq_version() {
    let (major, minor, patch) = zmq::version();
    println!("ZeroMQ version {}.{}.{}", major, minor, patch);
}
