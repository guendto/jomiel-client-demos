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

# See also Notes.md in php/docker/
#
FROM php:7.4-cli-alpine

WORKDIR /app

COPY php/docker/bin/ ./docker/bin/
COPY php/bin/demo ./bin/demo
COPY php/lib/ ./lib/

COPY php/composer.json .
COPY php/composer.lock .

COPY proto/ ./proto/

RUN set -euxo pipefail \
    ; apk --update --no-cache --virtual .build_deps add \
        zeromq-dev \
        php7-dev \
        libc-dev \
        protobuf \
        make \
        gcc \
        git \
        # for bootstrap script
        bash \
    # Download and install php-zmq from git repo.
    && sh ./docker/bin/getphpzmq.sh \
    # Produce protobuf declarations.
    && ./proto/bin/bootstrap -p proto/ -l php -d lib/proto/ \
    # Download the latest composer.
    && sh ./docker/bin/getcomposer.sh \
    # Install the php deps.
    && php ./composer.phar install --no-cache \
    # Install runtime deps.
    && apk --no-cache add \
        protobuf \
        zeromq \
    # Clean up.
    && rm -rf php-zmq composer.* docker/ bootstrap proto/ \
    && apk del .build_deps curl xz

ENTRYPOINT ["/app/bin/demo"]

CMD ["-h"]
