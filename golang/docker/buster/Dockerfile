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
FROM golang:1.15-buster AS bootstrap-image

WORKDIR /app

COPY golang/internal/ ./internal/
COPY golang/main.go ./
COPY golang/go.* ./
COPY proto/ ./proto/

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        protobuf-compiler \
        build-essential \
        libprotobuf-dev \
        libzmq3-dev \
        libczmq-dev \
        pkgconf \
        git \
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
FROM debian:buster-slim AS runtime-image

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        libprotobuf-lite17 \
        libprotobuf17 \
        libczmq4 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=compile-image /go/bin/demo /

ENTRYPOINT ["/demo"]

CMD ["-h"]
