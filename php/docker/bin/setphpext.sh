#!/bin/sh
#
# -*- coding: utf-8 -*-
#
# jomiel-client-demos
#
# Copyright
#  2021-2022 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# $1 = php7 or php8
export PHPINIDIR=`php -i \
    | grep -i 'conf.* path' \
    | awk '{print$NF}'` \
  && export PHPINI=$PHPINIDIR/php.ini \
  && cp $PHPINI-development $PHPINI \
  && echo "# jomiel-client-demos: dynamic extensions" >> $PHPINI \
  && echo "extension_dir=\"/usr/lib/$1/modules\"" >> $PHPINI \
  && echo "extension=zmq" >> $PHPINI
