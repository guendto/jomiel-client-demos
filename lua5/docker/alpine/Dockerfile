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
FROM alpine:3.13 AS build-image

WORKDIR /app

ENV ROCKSPEC demo-0.1.0-1.rockspec

COPY lua5/${ROCKSPEC} ./

RUN set -eux \
    # Install the build deps.
    ; apk --update --no-cache --virtual .build-dep add \
        luarocks5.3 \
        zeromq-dev \
        lua5.3-dev \
        musl-dev \
        yaml-dev \
        make \
        curl \
        bash \
        zip \
        gcc \
    # Install the Lua deps.
    && luarocks-5.3 build --tree ./local --only-deps ${ROCKSPEC} \
    # Build the .rock from each installed dep.
    && luarocks-5.3 list --tree ./local/ --porcelain | awk '{print $1}' \
        | xargs -I {} bash -c 'luarocks-5.3 pack --tree ./local {}' \
    # Move the .rock
    && mkdir -p rocks/ \
    && mv *.rock rocks/ \
    # Generate the manifest files for the .rock
    && luarocks-admin-5.3 make-manifest --tree ./local rocks/

# stage: runtime-image
#
FROM alpine:3.13 AS runtime-image

WORKDIR /app

ENV ROCKSPEC demo-0.1.0-1.rockspec

COPY lua5/${ROCKSPEC} ./

COPY lua5/bin/demo ./bin/demo
COPY lua5/src/ ./src/

COPY --from=build-image /app/rocks/ /app/rocks/
COPY proto/ ./proto/

RUN set -eux \
    ; apk --update --no-cache add \
    && apk --no-cache add \
        luarocks5.3 \
        yaml-dev \
        lua5.3 \
        zeromq \
    # Install the local .rocks
    && luarocks-5.3 build --tree ./local \
        --only-server rocks/ --only-deps ${ROCKSPEC} \
    # bin/demo: fix the shebang.
    && sed -i 's/env lua/env lua5.3/' bin/demo \
    # Cleanup.
    && apk del luarocks5.3 \
    && rm -rf rocks ${ROCKSPEC}

ENTRYPOINT ["/app/bin/demo", "--proto-dir", "./proto"]

CMD ["-h"]
