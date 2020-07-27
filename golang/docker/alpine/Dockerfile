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
FROM golang:1.14-alpine AS bootstrap-image

WORKDIR $GOPATH/src/jomiel.github.io/demo

COPY golang/src/jomiel.github.io/demo/ ./
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
FROM alpine:3.12 AS runtime-image

RUN set -eux \
    ; apk --update --no-cache add \
        libsodium \
        protobuf \
        zeromq \
        czmq

COPY --from=compile-image /go/bin/demo /

ENTRYPOINT ["/demo"]

CMD ["-h"]
