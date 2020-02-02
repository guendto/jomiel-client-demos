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
FROM alpine:3.10 AS bootstrap-image

WORKDIR /app

COPY tools/cmake ./cmake/
COPY proto/ ./proto/

COPY c/CMakeLists.txt .
COPY c/src/ ./src/

RUN set -eux \
    ; apk --update --no-cache add \
        protobuf-c-dev \
        protobuf-dev \
        protobuf-c \
        musl-dev \
        czmq-dev \
        pkgconf \
        cmake \
        make \
        gcc \
        # for bootstrap script
        bash \
    && sed -i 's/\.\.\/tools/\./' ./CMakeLists.txt

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; cmake . -DWITH_DOCKER=1 \
    && make

# stage: runtime-image
#
FROM alpine:3.10 AS runtime-image

RUN set -eux \
    ; apk --update --no-cache add \
        protobuf-c \
        protobuf \
        czmq

COPY --from=compile-image /app/demo /

ENTRYPOINT ["/demo"]

CMD ["-h"]
