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
FROM perl:5-slim-buster

WORKDIR /app

COPY perl5/bin/demo ./bin/demo

COPY perl5/cpanfile.snapshot .
COPY perl5/cpanfile .

COPY perl5/lib/ ./lib/
COPY proto/ ./proto/

ENV PERL_CPANM_OPT "--notest --quiet --force"

ENV BUILD_DEPS \
        libprotobuf-dev \
        build-essential \
        ca-certificates \
        libprotoc-dev \
        libzmq3-dev \
        libssl-dev \
        openssl

RUN set -eux \
    # Install the build deps.
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install $BUILD_DEPS \
    && sed -i 's/\.\.\/proto/\.\/proto/' lib/Jomiel.pm \
    # Install the perl runtime deps.
    && cpanm --notest --quiet Carton \
    && carton \
    # Cleanup (stage #1).
    && apt-get -qq purge $BUILD_DEPS \
    && apt-get -qq autoremove \
    # Install the runtime deps.
    && apt-get -qq --no-install-recommends install \
        libprotobuf-lite17 \
        libprotobuf17 \
        libzmq5 \
    # Cleanup (stage #2).
    && rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["perl", "-Ilocal/lib/perl5", "./bin/demo"]

CMD ["-h"]
