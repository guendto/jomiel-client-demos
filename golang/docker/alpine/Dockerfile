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
FROM golang:1.16-alpine AS bootstrap-image

WORKDIR /app

COPY golang/internal/ ./internal/
COPY golang/main.go ./
COPY golang/go.* ./
COPY proto/ ./proto/

RUN set -eux \
    ; apk --update --no-cache add \
        libsodium-dev \
        linux-headers \
        zeromq-dev \
        pkgconfig \
        czmq-dev \
        protobuf \
        musl-dev \
        gcc \
        git \
        # for bootstrap script
        bash \
    && sed -i 's/\.\.\/proto/\.\/proto/g' main.go \
    && go get -v google.golang.org/protobuf/cmd/protoc-gen-go \
    && go generate

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; go build -o /go/bin/demo

# stage: runtime-image
#
FROM alpine:3.13 AS runtime-image

RUN set -eux \
    ; apk --update --no-cache add \
        libsodium \
        protobuf \
        zeromq \
        czmq

COPY --from=compile-image /go/bin/demo /

ENTRYPOINT ["/demo"]

CMD ["-h"]
