# -*- coding: utf-8 -*-
#
# jomiel-client-demos
#
# Copyright
#  2019-2022 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# stage: bootstrap-image
#
FROM golang:1-bullseye AS bootstrap-image

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
    && go install google.golang.org/protobuf/cmd/protoc-gen-go@latest \
    && go generate

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; go build -o /go/bin/demo

# stage: runtime-image
#
FROM debian:bullseye-slim AS runtime-image

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        libprotobuf-lite23 \
        libprotobuf23 \
        libczmq4 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=compile-image /go/bin/demo /

ENTRYPOINT ["/demo"]

CMD ["-h"]
