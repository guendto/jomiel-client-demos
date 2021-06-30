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

# stage: bootstrap-image
#
FROM debian:buster-slim AS bootstrap-image

WORKDIR /app

COPY cpp/cmake/ ./cpp/cmake/
COPY cpp/src/ ./cpp/src/

COPY tools/ ./tools/
COPY proto/ ./proto/

RUN set -eux \
    # We want libfmt-dev 6.1 from buster-backports.
    ; echo 'deb http://deb.debian.org/debian buster-backports main' \
        >> /etc/apt/sources.list \
    && apt-get -qq update \
    && apt-get -t buster-backports -qq --no-install-recommends install \
        libfmt-dev \
    && apt-get -qq --no-install-recommends install \
        protobuf-compiler \
        ca-certificates \
        libprotobuf-dev \
        build-essential \
        libzmq3-dev \
        pkgconf \
        cmake \
        git \
    && rm -rf /var/lib/apt/lists/*

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; cd cpp \
    && mkdir build \
    && cd build \
    && cmake ../cmake -DWITH_DOCKER=ON \
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

COPY --from=compile-image /app/cpp/build/demo /

ENTRYPOINT ["/demo"]

CMD ["-h"]
