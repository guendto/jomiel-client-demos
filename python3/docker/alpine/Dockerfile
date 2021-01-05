# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2019-2020 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
FROM python:3.9-alpine

WORKDIR /app

COPY python3/requirements.txt .
COPY python3/demo/ ./demo/

COPY proto/ ./proto/

RUN set -eux \
    ; apk --update --no-cache --virtual .build_deps add \
        linux-headers \
        protobuf \
        g++ \
        # for bootstrap script
        bash \
    && ./proto/bin/bootstrap -p proto/ -l python -d demo/ \
    && pip install --no-cache-dir -r requirements.txt \
    && rm -rf proto/ requirements.txt \
    && apk del .build_deps \
    && apk add libzmq

ENTRYPOINT ["python", "/app/demo"]

CMD ["-h"]
