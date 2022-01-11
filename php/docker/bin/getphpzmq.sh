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

git clone https://github.com/zeromq/php-zmq \
  && cd php-zmq \
  && phpize \
  && ./configure \
  && make install
