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

use serde::Serialize;
use structopt::StructOpt;

#[derive(StructOpt, Serialize)]
#[structopt(name = "demo")]
pub struct Options {
    /// Print configuration values and exit
    #[structopt(short = "D", long = "print-config")]
    pub print_config: bool,

    /// Display ZeroMQ version and exit
    #[structopt(short = "V", long = "version-zmq")]
    pub version_zmq: bool,

    /// Print dumped messages in JSON
    #[structopt(short = "j", long = "output-json")]
    pub output_json: bool,

    /// Use more compact representation of JSON
    #[structopt(short = "c", long = "compact-json")]
    pub compact_json: bool,

    /// Specify the router endpoint address
    #[structopt(
        name = "addr",
        short = "r",
        long = "router-endpoint",
        default_value = "tcp://localhost:5514"
    )]
    pub router_endpoint: String,

    /// Specify maximum time in seconds for the connection allowed to take
    #[structopt(
        name = "time",
        short = "t",
        long = "connect-timeout",
        default_value = "30"
    )]
    pub connect_timeout: i64,

    /// Be brief and to the point; dump interesting details only
    #[structopt(short = "q", long = "be-terse")]
    pub be_terse: bool,

    /// the URIs to parse
    #[structopt()]
    pub uri: Vec<String>,
}
