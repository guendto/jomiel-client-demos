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
apk --update --no-cache --virtual .build_deps add \
    zeromq-dev \
    php7-dev \
    libc-dev \
    protobuf \
    make \
    gcc \
    git \
    # for bootstrap script
    bash
