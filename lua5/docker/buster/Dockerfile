# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2019 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
FROM debian:buster-slim

WORKDIR /app

COPY lua5/bin/ ./bin/
COPY lua5/lib/ ./lib/
COPY proto/ ./proto/

ENV BUILD_DEPS \
        build-essential \
        libzmq3-dev \
        luarocks \
        git

RUN set -eux \
    # Install the build deps.
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install $BUILD_DEPS \
    # Install the lua runtime deps.
    && eval "set -- lua-protobuf argparse serpent log4l lzmq"; \
        for dep; do \
            luarocks --local-tree install $dep || break; \
        done \
    # Modify the import path for .proto files.
    && sed -i 's/\.\.\/proto/\.\/proto/' lib/demo/jomiel.lua \
    # Cleanup (stage #1).
    && apt-get -qq purge $BUILD_DEPS \
    && apt-get -qq autoremove \
    # Install the required runtime.
    && apt-get -qq --no-install-recommends install \
        libzmq5 \
        lua5.1 \
    # Cleanup (stage #2).
    && rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["/app/bin/demo"]

CMD ["-h"]
