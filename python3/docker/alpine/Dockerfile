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
FROM python:3.8-alpine

WORKDIR /app

COPY python3/requirements.txt .
COPY python3/demo/ ./demo/

COPY proto/ ./proto/
COPY bin/bootstrap .

RUN set -eux \
    ; apk --update --no-cache --virtual .build_deps add \
        linux-headers \
        protobuf \
        g++ \
    && ./bootstrap -p proto/ -l python -d demo/proto/ \
    && pip install --no-cache-dir -r requirements.txt \
    && sed -i 's/\.\.\/proto/\.\/proto/' demo/app.py \
    && rm -rf ./bootstrap proto/ requirements.txt \
    && apk del .build_deps \
    && apk add libzmq

ENTRYPOINT ["python", "/app/demo"]

CMD ["-h"]
