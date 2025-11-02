/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2025 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */
pub mod jomiel {
    pub mod protobuf {
        pub mod v1beta1 {
            include!(concat!(
                env!("OUT_DIR"),
                "/jomiel.protobuf.v1beta1.rs"
            ));
        }
    }
}
