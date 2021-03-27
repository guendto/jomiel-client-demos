# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2019-2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# stage: bootstrap-image
#
FROM debian:buster-slim AS bootstrap-image

WORKDIR /app

COPY c/CMakeLists.txt ./c/
COPY c/src/ ./c/src/

COPY tools/ ./tools/
COPY proto/ ./proto/

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        protobuf-c-compiler \
        protobuf-compiler \
        libprotobuf-c-dev \
        libprotobuf-dev \
        build-essential \
        libczmq-dev \
        libpopt-dev \
        pkgconf \
        cmake \
    && rm -rf /var/lib/apt/lists/*

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; cd c \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make

# stage: runtime-image
#
FROM debian:buster-slim AS runtime-image

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        libprotobuf-c1 \
        libpopt0 \
        libczmq4 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=compile-image /app/c/build/demo /

ENTRYPOINT ["/demo"]

CMD ["-h"]
