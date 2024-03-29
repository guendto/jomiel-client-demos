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

# stage: bootstrap
#
FROM python:3-slim-bullseye AS bootstrap-image

WORKDIR /app

COPY python3/pyproject.toml .
COPY python3/setup.cfg .
COPY python3/setup.py .
COPY python3/src/ ./src/

RUN set -eux \
    # Build the jomiel-client-demo wheel from the source.
    ; pip wheel \
        . \
        --wheel-dir /app/wheels \
        --no-deps

# stage: runtime
#
# Use the "debian:slim" instead of "python:3-slim". The resulting image
# will be ~40MB smaller (for bullseye).
#
FROM debian:bullseye-slim AS runtime-image
# FROM python:3-slim-bullseye AS runtime-image

WORKDIR /app

COPY --from=bootstrap-image /app/wheels /wheels

ENV BUILD_DEPS \
    python3-pip

RUN set -eux \
    ; apt-get -qq update \
    # Install pip and install the wheel we built at bootstrap stage.
    && apt-get -qq --no-install-recommends install $BUILD_DEPS \
    && pip3 install --no-cache-dir /wheels/*.whl \
    # Remove pip (and all related packages, it'll be worth it).
    && apt-get -qq purge $BUILD_DEPS \
    && apt-get -qq autoremove \
    # And install python3 interpreter.
    && apt-get -qq --no-install-recommends install \
        python3 \
    # Every bit counts.
    && rm -rf /wheels \
    && rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["jomiel-client-demo"]

CMD ["-h"]
