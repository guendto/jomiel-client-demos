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
FROM node:14-alpine AS bootstrap-image

WORKDIR /app

COPY nodejs/bin/ ./bin/
COPY nodejs/lib/ ./lib/

COPY nodejs/pnpm-lock.yaml .
COPY nodejs/package.json .

COPY proto/ ./proto/

RUN set -eux \
    ; apk --update --no-cache add \
        zeromq-dev \
        musl-dev \
        python2 \
        make \
        g++ \
        # for gen-static script
        bash \
    && pnpm install \
    && ./bin/gen-static -p ./proto

ENTRYPOINT ["/app/demo"]

CMD ["-h"]
