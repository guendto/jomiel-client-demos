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

# stage: bootstrap-image
#
FROM gradle:6.8-jdk11-openj9 as bootstrap-image

WORKDIR /app

COPY java/src/ ./src/
COPY proto/ ./proto/

COPY java/settings.gradle .
COPY java/build.gradle .

RUN set -eux \
    ; apt-get -qq update \
    && apt-get install -qq --no-install-recommends \
        protobuf-compiler \
    && sed -i 's/\.\.\/proto/\.\/proto/' ./build.gradle

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; gradle build -q

# stage: runtime-image
#
# See also:
# - https://github.com/adoptopenjdk/openjdk-docker
FROM adoptopenjdk/openjdk11-openj9:alpine-slim as runtime-image

ENV JARFILE demo-0.1.0-SNAPSHOT-all.jar

COPY --from=compile-image /app/build/libs/${JARFILE} /

ENTRYPOINT ["/bin/sh", "-c", "java -jar $JARFILE $0 $@"]

CMD ["-h"]
