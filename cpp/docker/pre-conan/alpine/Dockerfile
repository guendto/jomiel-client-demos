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

# Note:
#   - alpine packages cppzmq (zmq.hpp) in edge [2019-09-22, 2020-07-27]
#   - Until that becomes available, workaround this
#

# stage: bootstrap-image
#
FROM alpine:3 AS bootstrap-image

WORKDIR /app

COPY cpp/cmake/ ./cpp/cmake/
COPY cpp/src/ ./cpp/src/

COPY tools/ ./tools/
COPY proto/ ./proto/

RUN set -eux \
    ; apk --update --no-cache add \
        ca-certificates \
        protobuf-dev \
        zeromq-dev \
        musl-dev \
        fmt-dev \
        pkgconf \
        # cppzmq \
        cmake \
        make \
        git \
        g++ \
    && cd cpp \
    # See the 'cppzmq' note above.
    #
    && mkdir -p ext/cppzmq \
    && cd ext/cppzmq \
        && wget -q https://raw.githubusercontent.com/zeromq/cppzmq/master/zmq.hpp \
        && cd ../.. \
        && sed -i 's/\Wzmq\.hpp\W/"cppzmq\/zmq.hpp"/' src/demo/jomiel.h

# stage: compile-image
#
FROM bootstrap-image AS compile-image

RUN set -eux \
    ; cd cpp \
    && mkdir build \
    && cd build \
    && cmake ../cmake -DWITH_DOCKER=ON \
    && make

# stage: runtime-image
#
FROM alpine:3 AS runtime-image

RUN set -eux \
    ; apk --update --no-cache add \
        protobuf \
        zeromq \
        fmt

COPY --from=compile-image /app/cpp/build/demo /

ENTRYPOINT ["/demo"]

CMD ["-h"]
