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

# stage: bootstrap-image
#
FROM alpine:3 AS bootstrap-image

WORKDIR /app

COPY python3/pyproject.toml .
COPY python3/setup.cfg .
COPY python3/setup.py .
COPY python3/src/ ./src/

RUN set -eux \
    ; apk --update --no-cache --virtual .build_deps add \
        py3-pip \
    && pip wheel \
        . \
        --wheel-dir /app/wheels \
        --no-deps \
    && apk del .build_deps

# runtime-image

# stage: runtime-image
#
FROM alpine:3 AS runtime-image

WORKDIR /app

COPY --from=bootstrap-image /app/wheels /wheels

RUN set -eux \
    ; apk --update --no-cache --virtual .build_deps add \
        py3-pyzmq \
        py3-pip \
    && pip install --no-cache /wheels/*.whl \
    && apk del py3-pip

ENTRYPOINT ["jomiel-examples-demo"]

CMD ["-h"]
