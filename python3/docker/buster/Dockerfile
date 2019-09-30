# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2019 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
FROM python:3.7-slim

WORKDIR /app

COPY python3/requirements.txt .
COPY python3/demo/ ./demo/

COPY proto/ ./proto/
COPY bin/bootstrap .

ENV BUILD_DEPS \
        protobuf-compiler \
        libprotobuf-dev \
        build-essential

RUN set -eux \
    # Install the build deps.
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install $BUILD_DEPS \
    # Install the python runtime modules.
    && ./bootstrap -p proto/ -l python -d demo/proto/ \
    && pip install --no-cache-dir -r requirements.txt \
    # Modify the import path.
    && sed -i 's/\.\.\/proto/\.\/proto/' demo/app.py \
    # Cleanup (stage #1).
    && rm -rf ./bootstrap proto/ requirements.txt \
    && apt-get -qq purge $BUILD_DEPS \
    && apt-get -qq autoremove \
    # Install the required runtime.
    && apt-get -qq --no-install-recommends install \
        libzmq5 \
    # Cleanup (stage #2).
    && rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["python", "demo"]

CMD ["-h"]
