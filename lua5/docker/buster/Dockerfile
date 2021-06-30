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

# stage: build-image
#
FROM debian:buster-slim AS build-image

WORKDIR /app

ENV ROCKSPEC demo-0.1.0-1.rockspec

COPY lua5/${ROCKSPEC} ./

ENV BUILD_DEPS \
  build-essential \
  liblua5.3-dev \
  libyaml-dev \
  libzmq3-dev \
  luarocks \
  lua5.3

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install $BUILD_DEPS \
    # Install the Lua deps.
    && luarocks build --tree ./local --only-deps ${ROCKSPEC} \
    # Build the .rock from each installed dep.
    && luarocks list --tree ./local/ --porcelain | awk '{print $1}' \
        | xargs -I {} bash -c 'luarocks pack --tree ./local {}' \
    # Move the .rock
    && mkdir -p rocks/ \
    && mv *.rock rocks/ \
    # Generate the manifest files for the .rock
    && luarocks-admin make-manifest --tree ./local rocks/

# stage: runtime-image
#
FROM debian:buster-slim AS runtime-image

WORKDIR /app

ENV ROCKSPEC demo-0.1.0-1.rockspec

COPY lua5/${ROCKSPEC} ./

COPY lua5/bin/demo ./bin/demo
COPY lua5/src/ ./src/

COPY --from=build-image /app/rocks/ /app/rocks/
COPY proto/ ./proto/

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        libyaml-dev \
        luarocks \
        libzmq5 \
        lua5.3 \
    # Install the local .rocks
    && luarocks build --tree ./local \
        --only-server rocks/ --only-deps ${ROCKSPEC} \
    # Cleanup.
    && rm -rf /var/lib/apt/lists/* \
    && apt-get purge luarocks -y \
    && rm -rf rocks ${ROCKSPEC}

ENTRYPOINT ["/app/bin/demo", "--proto-dir", "./proto"]

CMD ["-h"]
