#!/bin/sh
#
# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# See also:
# - docker/Notes.md
#
git clone https://github.com/zeromq/php-zmq \
  && cd php-zmq \
  && phpize \
  && ./configure \
  && make install \
  && export PHPINIDIR=`php -i \
    | grep -i 'conf.* path' \
    | awk '{print$NF}'` \
  && cp $PHPINIDIR/php.ini-development $PHPINIDIR/php.ini \
  && sed -i 's/;extension=bz2/extension=zmq/' $PHPINIDIR/php.ini
