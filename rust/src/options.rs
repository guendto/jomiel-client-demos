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

use structopt::StructOpt;

#[derive(StructOpt, Debug)]
#[structopt(name = "demo")]
pub struct Options {
    /// Specify maximum time in seconds for the connection allowed to take
    #[structopt(
        name = "time",
        short = "t",
        long = "connect-timeout",
        default_value = "30"
    )]
    pub connect_timeout: i64,

    /// Specify the router endpoint address
    #[structopt(
        name = "addr",
        short = "r",
        long = "router-endpoint",
        default_value = "tcp://localhost:5570"
    )]
    pub router_endpoint: String,

    /// Print configuration values and exit
    #[structopt(short = "D", long = "print-config")]
    pub print_config: bool,

    /// Display ZeroMQ version and exit
    #[structopt(short = "V", long = "version-zmq")]
    pub version_zmq: bool,

    /*
    /// Print dumped messages in JSON
    #[structopt(short = "j", long = "output-json")]
    pub output_json: bool,
    */
    /// Be brief and to the point; dump interesting details only
    #[structopt(short = "q", long = "be-terse")]
    pub be_terse: bool,

    /// the URIs to parse
    #[structopt()]
    pub uri: Vec<String>,
}
