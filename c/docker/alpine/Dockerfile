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
FROM alpine:3.10 AS bootstrap-image

WORKDIR /app

COPY proto/ ./proto/
COPY bin/bootstrap .

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
    && ./bootstrap -p proto -l c -d src/demo/proto/

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; cmake . \
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
