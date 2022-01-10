# -*- coding: utf-8 -*-
#
# jomiel-client-demos
#
# Copyright
#  2019-2022 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
FROM node:current-alpine

WORKDIR /app

COPY nodejs/package.json .
COPY nodejs/src/ ./src/

RUN set -eux \
    ; npm i -q --production

ENTRYPOINT ["node", "."]

CMD ["-h"]
