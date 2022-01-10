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

# stage: bootstrap-image
#
FROM maven:3-eclipse-temurin-11-alpine AS bootstrap-image

WORKDIR /app

COPY java/src/ ./src/
COPY java/pom.xml .

COPY proto/ ./proto/

RUN set -eux \
    ; apk --update --no-cache add \
        protoc \
    && sed -i 's/\.\.\/proto/\.\/proto/' ./pom.xml

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; mvn -q -B package

# stage: runtime-image
#
FROM eclipse-temurin:11-alpine AS runtime-image

COPY --from=compile-image /app/target/demo-1.0-SNAPSHOT.jar /

ENTRYPOINT ["java", "-jar", "demo-1.0-SNAPSHOT.jar"]

CMD ["-h"]
