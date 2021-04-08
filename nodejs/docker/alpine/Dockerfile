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
COPY nodejs/bin/ ./bin/
COPY nodejs/src/ ./src/
COPY proto/ ./proto/

RUN set -eux \
    ; npm i -q \
    && sed -i 's/\.\.\/proto/\.\/proto/' package.json \
    && npm run gen \
    && rm -rf ./bin ./proto

ENTRYPOINT ["node", "."]

CMD ["-h"]
