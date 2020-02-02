# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2019-2020 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# stage: bootstrap-image
#
FROM debian:buster-slim AS bootstrap-image

WORKDIR /app

COPY c/CMakeLists.txt .
COPY c/src/ ./src/

COPY tools/cmake ./cmake/
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
        pkgconf \
        cmake \
    && rm -rf /var/lib/apt/lists/* \
    && sed -i 's/\.\.\/tools/\./' ./CMakeLists.txt

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; cmake . -DWITH_DOCKER=1 \
    && make

# stage: runtime-image
#
FROM debian:buster-slim AS runtime-image

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        libprotobuf-c1 \
        libczmq4 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=compile-image /app/demo /

ENTRYPOINT ["/demo"]

CMD ["-h"]
