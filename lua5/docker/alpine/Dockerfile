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
FROM alpine:3.12

WORKDIR /app

COPY lua5/bin/ ./bin/
COPY lua5/lib/ ./lib/

COPY proto/ ./proto/

RUN set -eux \
    # Install the build deps.
    ; apk --update --no-cache --virtual .build-dep add \
        luarocks5.3 \
        zeromq-dev \
        lua5.3-dev \
        musl-dev \
        make \
        curl \
        gcc \
        git  \
    # Install the lua runtime deps.
    && eval "set -- lua-protobuf argparse serpent log4l lzmq"; \
        for dep; do \
            luarocks-5.3 --local-tree install $dep || break; \
        done \
    # Modify paths to run in a container.
    && sed -i 's/\.\.\/proto/\.\/proto/' lib/demo/jomiel.lua \
    && sed -i 's/env lua/env lua5.3/' bin/demo \
    # Cleanup.
    && apk del .build-dep \
    # Install the runtime deps.
    && apk --no-cache add \
        lua5.3 \
        zeromq

ENTRYPOINT ["/app/bin/demo"]

CMD ["-h"]
