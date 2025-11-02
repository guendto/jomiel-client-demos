/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2019-2021,2025 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */
use clap::Parser;
use serde::Serialize;

#[derive(Parser, Serialize)]
#[command(name = "demo")]
pub struct Options {
    /// Print configuration values and exit
    #[arg(short = 'D', long = "print-config")]
    pub print_config: bool,

    /// Display ZeroMQ version and exit
    #[arg(short = 'V', long = "version-zmq")]
    pub version_zmq: bool,

    /// Print dumped messages in JSON
    #[arg(short = 'j', long = "output-json")]
    pub output_json: bool,

    /// Use more compact representation of JSON
    #[arg(short = 'c', long = "compact-json")]
    pub compact_json: bool,

    /// Specify the router endpoint address
    #[arg(
        value_name = "addr",
        short = 'r',
        long = "router-endpoint",
        default_value = "tcp://localhost:5514"
    )]
    pub router_endpoint: String,

    /// Specify maximum time in seconds for the connection allowed to take
    #[arg(
        value_name = "time",
        short = 't',
        long = "connect-timeout",
        default_value = "30"
    )]
    pub connect_timeout: i64,

    /// Be brief and to the point; dump interesting details only
    #[arg(short = 'q', long = "be-terse")]
    pub be_terse: bool,

    /// the URIs to parse
    #[arg()]
    pub uri: Vec<String>,
}
