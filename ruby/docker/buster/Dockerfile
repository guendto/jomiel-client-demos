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
FROM ruby:2.6-slim

WORKDIR /app

COPY ruby/Gemfile.lock .
COPY ruby/Gemfile .

COPY ruby/lib/ ./lib/
COPY ruby/bin/ ./bin/

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
    # Install the ruby runtime modules.
    && ./bootstrap -p proto/ -l ruby -d lib/ \
    && bundler install \
    # Cleanup (stage #1).
    && rm -rf ./bootstrap proto/ Gemfile Gemfile.lock \
    && apt-get -qq purge $BUILD_DEPS \
    && apt-get -qq autoremove \
    # Install the required runtime.
    # Note: 'libzmq5' package is not enough (why? ruby related?)
    #           ("The lib zmq library (or DLL) could not be loaded")
    && apt-get -qq --no-install-recommends install libzmq3-dev \
    # Cleanup (stage #2).
    && rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["/app/bin/demo"]

CMD ["-h"]
