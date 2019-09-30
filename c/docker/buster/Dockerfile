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

# stage: bootstrap-image
#
FROM debian:buster-slim AS bootstrap-image

WORKDIR /app

COPY c/CMakeLists.txt .
COPY c/src/ ./src/

COPY proto/ ./proto/
COPY bin/bootstrap .

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        protobuf-c-compiler \
        libprotobuf-c-dev \
        libprotobuf-dev \
        build-essential \
        libczmq-dev \
        pkgconf \
        cmake \
    && rm -rf /var/lib/apt/lists/* \
    && ./bootstrap -p proto -l c -d src/demo/proto/

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; cmake . \
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
