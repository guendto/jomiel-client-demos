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

# NOTES:
# - 'bash' is installed for the bootstrap script
#

# $1 = php7 or php8
apk --update --no-cache --virtual .build_deps add \
    $1-dev \
    zeromq-dev \
    libc-dev \
    protobuf \
    make \
    gcc \
    git \
    bash
