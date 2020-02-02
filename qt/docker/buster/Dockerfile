# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2020 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# stage: bootstrap-image
#
FROM debian:buster-slim AS bootstrap-image

WORKDIR /app

COPY qt/patches/ ./patches/
COPY qt/CMakeLists.txt .
COPY qt/src/ ./src/

COPY tools/cmake ./cmake/
COPY proto/ ./proto/

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        protobuf-compiler \
        ca-certificates \
        libprotobuf-dev \
        build-essential \
        libzmq3-dev \
        pkgconf \
        cmake \
        git \
        # qt
        qtbase5-dev \
        libqt5opengl5-dev \
        qt5-default \
    && rm -rf /var/lib/apt/lists/* \
    && sed -i 's/\.\.\/tools/\./' ./CMakeLists.txt

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; cmake -DWITH_DOCKER=ON . \
    && make

# state: runtime-image
#
FROM debian:buster-slim AS runtime-image

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        libprotobuf-lite17 \
        libprotobuf17 \
        libzmq5 \
        # qt
        libqt5widgets5 \
        libqt5core5a \
        libqt5gui5 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=compile-image /app/demo /

ENTRYPOINT ["/demo"]
