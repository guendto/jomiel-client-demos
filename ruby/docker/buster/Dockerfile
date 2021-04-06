# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2019-2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
FROM ruby:2.7-slim

WORKDIR /app

COPY ruby/Gemfile.lock .
COPY ruby/Gemfile .

COPY ruby/src/ ./src/
COPY ruby/bin/ ./bin/

COPY proto/ ./proto/

ENV BUILD_DEPS \
        protobuf-compiler \
        libprotobuf-dev \
        build-essential

RUN set -eux \
    # We want protobuf-compiler 3.12 from buster-backports.
    #
    ; echo 'deb http://deb.debian.org/debian buster-backports main' \
        >> /etc/apt/sources.list \
    # Install the build deps.
    && apt-get -qq update \
    && apt-get -t buster-backports -qq --no-install-recommends install \
        $BUILD_DEPS \
    # Install the ruby runtime modules.
    && ./proto/bin/bootstrap -p proto/ -l ruby -d src/ \
    && bundler install \
    # Cleanup (stage #1).
    && rm -rf proto/ Gemfile Gemfile.lock \
    && apt-get -qq purge $BUILD_DEPS \
    && apt-get -qq autoremove \
    # Install the required runtime.
    && apt-get -t buster-backports -qq --no-install-recommends install \
        libzmq3-dev \
    # Cleanup (stage #2).
    && rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["/app/bin/demo"]

CMD ["-h"]
