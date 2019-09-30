# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2019 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# stage: bootstrap-image
#
FROM node:12-alpine AS bootstrap-image

WORKDIR /app

COPY nodejs/bin/demo ./bin/demo
COPY nodejs/lib/ ./lib/

COPY nodejs/package.json .
COPY nodejs/yarn.lock .

COPY proto/ ./proto/

RUN set -eux \
    ; apk --update --no-cache add \
        zeromq-dev \
        musl-dev \
        python2 \
        make \
        g++ \
    && yarn install --no-lockfile \
    && sed -i 's/\.\.\/proto/\.\/proto/' lib/jomiel.js

# stage: runtime-image
#
FROM node:12-alpine AS runtime-image

WORKDIR /app

COPY --from=bootstrap-image /app/node_modules/ ./node_modules/
COPY --from=bootstrap-image /app/bin/demo .

COPY --from=bootstrap-image /app/proto/ ./proto/
COPY --from=bootstrap-image /app/lib/ ./lib/

ENTRYPOINT ["/app/demo"]

CMD ["-h"]
