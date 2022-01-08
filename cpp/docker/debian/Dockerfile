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
FROM debian:bullseye-slim AS bootstrap-image

WORKDIR /app

COPY cpp/CMakeLists.txt ./cpp/
COPY cpp/conanfile.txt ./cpp/
COPY cpp/src/ ./cpp/src/

COPY tools/ ./tools/
COPY proto/ ./proto/

RUN set -eux \
    ; apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        python3-setuptools \
        python3-pip \
        build-essential \
        cmake \
    && pip3 install -q conan \
    && conan profile new default --detect \
    && conan profile update \
        settings.compiler.libcxx=libstdc++11 default \
    && rm -rf /var/lib/apt/lists/*

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; cd cpp \
    && mkdir build \
    && cd build \
    && conan install .. \
    && cmake .. \
    && make

# stage: runtime-image
#
FROM debian:bullseye-slim AS runtime-image

COPY --from=compile-image /app/cpp/build/bin/demo /

ENTRYPOINT ["/demo"]

CMD ["-h"]
