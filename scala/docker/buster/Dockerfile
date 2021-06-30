# -*- coding: utf-8 -*-
#
# jomiel-client-demos
#
# Copyright
#  2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# stage: bootstrap-image
#
FROM gradle:6.8-jdk11-openj9 as bootstrap-image

WORKDIR /app

COPY scala/src/ ./src/
COPY proto/ ./proto/

COPY scala/settings.gradle.kts .
COPY scala/build.gradle.kts .

RUN set -eux \
    ; apt-get -qq update \
    && apt-get install -qq --no-install-recommends \
        protobuf-compiler \
    && sed -i 's/\.\.\/proto/\.\/proto/' ./build.gradle.kts

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; gradle build -q

# stage: runtime-image
#
# See also:
# - https://github.com/adoptopenjdk/openjdk-docker
#
FROM adoptopenjdk/openjdk11-openj9:alpine-slim as runtime-image

WORKDIR /app

ENV TARFILE demo.tar

COPY --from=compile-image /app/build/distributions/${TARFILE} /app

RUN set eux \
    ; tar xf $TARFILE \
    && rm -f $TARFILE

ENTRYPOINT ["/app/demo/bin/demo"]

CMD ["-h"]
