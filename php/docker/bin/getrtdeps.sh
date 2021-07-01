#!/bin/sh
#
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

# $1 = php7 or php8
PHP8_REPO=http://dl-cdn.alpinelinux.org/alpine/edge/testing/
PHP_EXT="$1-sodium"

# alpine 3.13 does not package php8-pecl-zmq, it's in testing.
# - install php7-pecl-zmq ony, for php8-pecl-zmq, see further down
[ $1 = "php7" ] && PHP_EXT="$PHP_EXT $1-pecl-zmq"

apk --no-cache add $PHP_EXT protobuf zeromq

# install php8-pecl-zmq from edge/testing.
if [ $? -eq 0 ]; then
    if [ $1 = "php8" ]; then
        apk --no-cache add php8-pecl-zmq --repository=$PHP8_REPO
    fi
fi

exit $?
