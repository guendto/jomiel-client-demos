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

# NOTES:
# - 'bash' is installed for the bootstrap script
#
apk --update --no-cache --virtual .build_deps add \
    zeromq-dev \
    $1\
    libc-dev \
    protobuf \
    make \
    gcc \
    git \
    bash
