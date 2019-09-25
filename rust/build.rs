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

extern crate protoc_rust;

use protoc_rust::Customize;

fn main() {
    protoc_rust::run(protoc_rust::Args {
        out_dir: "src/proto",
        input: &[
            "../proto/Message.proto",
            "../proto/Media.proto",
            "../proto/Status.proto",
        ],
        includes: &["../proto"],
        customize: Customize {
            //serde_derive: Some(true),
            ..Default::default()
        },
    })
    .expect("protoc");
}
