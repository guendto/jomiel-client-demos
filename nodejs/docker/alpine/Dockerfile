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
FROM node:14-alpine

WORKDIR /app

COPY nodejs/package.json .
COPY nodejs/src/ ./src/

RUN set -eux \
    ; npm i -q

ENTRYPOINT ["node", "."]

CMD ["-h"]
