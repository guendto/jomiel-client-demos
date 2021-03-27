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
FROM alpine:3.13 AS bootstrap-image

WORKDIR /app

COPY c/CMakeLists.txt ./c/
COPY c/src/ ./c/src/

COPY tools/ ./tools/
COPY proto/ ./proto/

RUN set -eux \
    ; apk --update --no-cache add \
        protobuf-c-dev \
        protobuf-dev \
        protobuf-c \
        musl-dev \
        czmq-dev \
        popt-dev \
        pkgconf \
        cmake \
        make \
        gcc

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
FROM alpine:3.13 AS runtime-image

RUN set -eux \
    ; apk --update --no-cache add \
        protobuf-c \
        protobuf \
        popt \
        czmq

COPY --from=compile-image /app/c/build/demo /

ENTRYPOINT ["/demo"]

CMD ["--help"]
