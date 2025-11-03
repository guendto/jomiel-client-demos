# -*- coding: utf-8 -*-
#
# jomiel-client-demos
#
# Copyright
#  2019-2022,2025 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# stage: bootstrap-image
#
FROM rust:1-slim-bookworm AS bootstrap-image

WORKDIR /app

COPY rust/Cargo.toml .
COPY rust/Cargo.lock .
COPY rust/src/ ./src/
COPY rust/build.rs .
COPY proto/ ./proto/

RUN set -eux \
    && apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        protobuf-compiler \
        libzmq3-dev \
        pkgconf \
        g++ \
    && sed -i 's/\.\.\/proto/\.\/proto/' build.rs

FROM bootstrap-image AS compile-image
RUN set -eux \
    ; cargo build --release

FROM debian:bookworm-slim AS runtime-image
RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        libzmq5 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=compile-image /app/target/release/demo /app/demo

ENTRYPOINT ["/app/demo"]
CMD ["-h"]
