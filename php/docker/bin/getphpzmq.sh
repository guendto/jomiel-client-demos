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
  && make install
