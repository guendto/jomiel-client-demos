/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2021,2025 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */
use super::jomiel::Jomiel;
use super::options::Options;
use clap::Parser;

pub fn main() {
    let opts: Options = Options::parse();

    if opts.print_config {
        dump_config(&opts);
    } else if opts.version_zmq {
        print_zmq_version();
    } else {
        Jomiel::new(opts).inquire();
    }
}

fn dump_config(opts: &Options) {
    print!("{}", serde_json::to_string_pretty(opts).unwrap());
}

fn print_zmq_version() {
    let (major, minor, patch) = zmq::version();
    println!("ZeroMQ version {}.{}.{}", major, minor, patch);
}
