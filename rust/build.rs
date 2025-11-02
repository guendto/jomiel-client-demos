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
use glob::glob;

fn main() {
    let proto_files: Vec<_> = glob("../proto/**/*.proto")
        .expect("glob pattern failed")
        .map(|p| p.expect("bad path"))
        .collect();

    let mut config: prost_build::Config = prost_build::Config::new();

    config.type_attribute(
        ".",
        "#[derive(serde::Serialize, serde::Deserialize)]",
    );

    config
        .compile_protos(&proto_files, &["../proto"])
        .expect("prost-build failed");
}
