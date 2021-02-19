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

COPY cpp/CMakeLists.txt .
COPY cpp/src/ ./src/

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
    && rm -rf /var/lib/apt/lists/* \
    && sed -i 's/\.\.\/tools/\./' ./CMakeLists.txt

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; cmake -DWITH_DOCKER=ON . \
    && make

# stage: runtime-image
#
FROM debian:buster-slim AS runtime-image

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        libprotobuf-lite17 \
        libprotobuf17 \
        libzmq5 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=compile-image /app/demo /

ENTRYPOINT ["/demo"]

CMD ["-h"]
