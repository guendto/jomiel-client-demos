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

# Notes
#   - php-zmq
#       - php-zmq exists in PECL; it's outdated and has issues w/ php7
#           - https://pecl.php.net/package/zmq
#           - https://github.com/zeromq/php-zmq/issues/203
#           - https://github.com/zeromq/php-zmq/issues/193
#       - github.com/zeromq/php-zmq:master works (2019-09-20)
#           - phpbrew ext install github:zeromq/php-zmq master
#               - Not ideal because
#                   - We want to use composer (packagist.org)
#                   - Needs phpbrew installation
#       - Debian has php-zmq package
#           - Which is now (as of buster?) non-existing?
#

FROM php:7.4-cli-alpine

WORKDIR /app

COPY php/bin/demo ./bin/demo
COPY php/lib/ ./lib/

COPY php/composer.json .
COPY php/composer.lock .

COPY proto/ ./proto/

ARG CMPSR_URI=https://raw.githubusercontent.com/composer/getcomposer.org/76a7060/web/installer

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
    && git clone https://github.com/zeromq/php-zmq \
        && cd php-zmq \
            && phpize \
            && ./configure \
            && make install \
            && export PHPINIDIR=`php -i \
                | grep -i 'conf.* path' \
                | awk '{print$NF}'` \
            && cp $PHPINIDIR/php.ini-development $PHPINIDIR/php.ini \
            && sed -i 's/;extension=bz2/extension=zmq/' $PHPINIDIR/php.ini \
            && cd .. \
    # Produce protobuf declarations.
    && ./proto/bin/bootstrap -p proto/ -l php -d lib/proto/ \
    # Download composer and install deps.
    && wget $CMPSR_URI -O - -q | php -- --quiet \
    && php ./composer.phar install --no-cache \
    # Install runtime deps.
    && apk --no-cache add \
        protobuf \
        zeromq \
    # Clean up.
    && rm -rf php-zmq composer.* bootstrap proto/ \
    && apk del .build_deps curl xz

ENTRYPOINT ["/app/bin/demo"]

CMD ["-h"]
