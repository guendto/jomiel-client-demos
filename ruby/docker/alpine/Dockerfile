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
FROM ruby:3.0-alpine

WORKDIR /app

COPY ruby/Gemfile.lock .
COPY ruby/Gemfile .

COPY ruby/src/ ./src/
COPY ruby/bin/ ./bin/

COPY proto/ ./proto/

RUN set -eux \
    # Install the build deps.
    ; apk --update --no-cache --virtual .build_deps add \
        zeromq-dev \
        protobuf \
        musl-dev \
        make \
        gcc \
        # for bootstrap script
        bash \
    # Install the ruby runtime modules.
    && ./proto/bin/bootstrap -p proto/ -l ruby -d src/ \
    && bundle install \
    # Workaround (build the module):
    #   https://github.com/protocolbuffers/protobuf/issues/4460
    && gem uninstall -I google-protobuf \
    && gem install google-protobuf --platform=ruby \
    # Cleanup.
    && rm -rf proto/ Gemfile Gemfile.lock \
    && apk del .build_deps \
    # Install the runtime deps.
    #   Note: 'libzmq' package not enough (why? ruby related?)
    #           ("The lib zmq library (or DLL) could not be loaded")
    && apk add --no-cache zeromq-dev

ENTRYPOINT ["/app/bin/demo"]

CMD ["-h"]
