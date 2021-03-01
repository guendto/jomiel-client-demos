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
FROM maven:3-jdk-8-slim AS bootstrap-image
#FROM maven:3-jdk-11-slim AS bootstrap-image

WORKDIR /app

COPY java/src/ ./src/
COPY java/pom.xml .

COPY proto/ ./proto/

RUN set -eux \
    ; apt-get -qq update \
    && apt-get install -qq --no-install-recommends \
        protobuf-compiler \
        libatomic1 \
    && rm -rf /var/lib/apt/lists/* \
    && sed -i 's/\.\.\/proto/\.\/proto/' ./pom.xml

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; mvn -B package

# stage: runtime-image
#
FROM openjdk:11-jre-slim AS runtime-image
#FROM openjdk:14-alpine AS runtime-image

COPY --from=compile-image /app/target/demo-1.0-SNAPSHOT.jar /

ENTRYPOINT ["java", "-jar", "demo-1.0-SNAPSHOT.jar"]

CMD ["-h"]
