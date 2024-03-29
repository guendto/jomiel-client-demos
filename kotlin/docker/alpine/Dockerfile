# -*- coding: utf-8 -*-
#
# jomiel-client-demos
#
# Copyright
#  2021-2022 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# stage: bootstrap-image
#
FROM gradle:7-jdk11-alpine as bootstrap-image

WORKDIR /app

COPY kotlin/src/ ./src/
COPY proto/ ./proto/

COPY kotlin/settings.gradle.kts .
COPY kotlin/build.gradle.kts .

RUN set -eux \
    ; apk --update --no-cache add  \
        protoc \
    && sed -i 's/\.\.\/proto/\.\/proto/' ./build.gradle.kts

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; gradle build -q

# stage: runtime-image
#
FROM eclipse-temurin:11-alpine AS runtime_image

WORKDIR /app

ENV TARFILE demo.tar

COPY --from=compile-image /app/build/distributions/${TARFILE} /app

RUN set eux \
    ; tar xf $TARFILE \
    && rm -f $TARFILE

ENTRYPOINT ["/app/demo/bin/demo"]

CMD ["-h"]
