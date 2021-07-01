# -*- coding: utf-8 -*-
#
# jomiel-client-demos
#
# Copyright
#  2019-2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
FROM perl:5-slim-buster

WORKDIR /app

COPY perl5/bin/demo ./bin/demo

COPY perl5/cpanfile.snapshot .
COPY perl5/cpanfile .

COPY perl5/src/ ./src/
COPY proto/ ./proto/

ENV BUILD_DEPS \
    protobuf-compiler \
    libprotobuf-dev \
    build-essential \
    ca-certificates \
    libprotoc-dev \
    libzmq3-dev \
    libssl-dev \
    openssl

RUN set -eux \
    # Install the build deps.
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install $BUILD_DEPS \
    # Install carton.
    && cpanm --notest --quiet Carton \
    # Install the perl runtime deps.
    && carton install --without develop \
    # Generate protobuf bindings for the jomiel messages.
    && carton exec ./bin/demo --static-gen --proto-dir ./proto \
    # Cleanup (stage #1).
    && apt-get -qq purge $BUILD_DEPS \
    && apt-get -qq autoremove \
    # Install the runtime deps.
    && apt-get -qq --no-install-recommends install \
        libprotobuf-lite17 \
        libprotobuf17 \
        libzmq5 \
    # Cleanup (stage #2).
    && rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["./bin/demo"]

CMD ["-h"]
