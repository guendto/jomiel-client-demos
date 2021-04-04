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

extern crate glob;
extern crate protoc_rust;

use protoc_rust::Customize;
use std::iter::FromIterator;

fn main() {
    let r = glob_simple("../proto/**/*.proto");

    // https://stackoverflow.com/a/37470838, kudos.
    let input_files = Vec::from_iter(r.iter().map(String::as_str));

    protoc_rust::Codegen::new()
        .out_dir("src/proto")
        .inputs(&input_files)
        .include("../proto")
        .customize(Customize {
            serde_derive: Some(true),
            ..Default::default()
        })
        .run()
        .expect("protoc");
}

/* https://git.io/JvY72, kudos. */
pub fn glob_simple(pattern: &str) -> Vec<String> {
    let mut r: Vec<_> = glob::glob(pattern)
        .expect("glob")
        .map(|g| {
            g.expect("item")
                .as_path()
                .to_str()
                .expect("utf-8")
                .to_owned()
        })
        .collect();
    r.sort();
    r
}
