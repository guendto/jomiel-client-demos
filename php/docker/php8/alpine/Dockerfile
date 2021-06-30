# -*- coding: utf-8 -*-
#
# jomiel-client-demos
#
# Copyright
#  2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# See also Notes.md in php/docker/
#
FROM php:8-cli-alpine

WORKDIR /app

COPY php/docker/bin/ ./docker/bin/
COPY proto/ ./proto/

COPY php/bin/demo ./bin/demo
COPY php/src/ ./src/

COPY php/composer.json .
COPY php/composer.lock .

RUN set -euxo pipefail \
    # Install build deps.
    ; sh ./docker/bin/getbuilddeps.sh php8 \
    # Download and install php-zmq from git repo.
    # - commented out (install from alpine repo).
    # && sh ./docker/bin/getphpzmq.sh \
    # Enable extensions in php.ini
    && sh ./docker/bin/setphpext.sh php8 \
    # Install runtime deps.
    && sh ./docker/bin/getrtdeps.sh php8 \
    # Produce protobuf declarations.
    && ./proto/bin/bootstrap -p proto/ -l php -d src/build/gen/ \
    # Download the latest composer.
    && sh ./docker/bin/getcomposer.sh \
    # Install the php deps.
    && php ./composer.phar install -q --no-cache --no-dev \
    # Clean up.
    && sh ./docker/bin/cleanup.sh

ENTRYPOINT ["/app/bin/demo"]

CMD ["-h"]
