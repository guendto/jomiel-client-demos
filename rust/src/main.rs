/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

extern crate protobuf;
extern crate structopt;
extern crate zmq;

/*
extern crate serde;
extern crate serde_derive;
extern crate serde_json;
*/

use jomiel::Jomiel;

mod jomiel;
mod options;
mod proto;

fn main() {
    let jomiel = Jomiel::new();

    if jomiel.opts.print_config {
        print_config(&jomiel);
    } else if jomiel.opts.version_zmq {
        print_zmq_version()
    }

    if jomiel.opts.uri.is_empty() {
        println!("error: input URI not given");
        std::process::exit(1);
    }

    jomiel.connect();

    for uri in &jomiel.opts.uri {
        jomiel.inquire(uri);
    }
}

fn print_config(jomiel: &Jomiel) {
    println!("{:#?}", jomiel.opts);
    std::process::exit(0);
}

fn print_zmq_version() {
    let (major, minor, patch) = zmq::version();
    println!("ZeroMQ version {}.{}.{}", major, minor, patch);
    std::process::exit(0);
}
