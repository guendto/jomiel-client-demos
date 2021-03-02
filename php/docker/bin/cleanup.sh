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
rm -rf php-zmq composer.* docker/ bootstrap proto/ \
    && apk del .build_deps curl xz
