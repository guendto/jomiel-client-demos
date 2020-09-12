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
FROM golang:1.15-buster AS bootstrap-image

WORKDIR $GOPATH/src/jomiel.github.io/demo

COPY golang/src/jomiel.github.io/demo/ ./
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
    && go get -v \
        github.com/golang/protobuf/protoc-gen-go \
        gopkg.in/zeromq/goczmq.v4 \
        github.com/ghodss/yaml \
        github.com/spf13/pflag \
    && ./proto/bin/bootstrap -p proto -l go -d examples/proto/

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
