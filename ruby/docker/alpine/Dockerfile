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

# alpine 3.10 packages protobuf 3.6.1-r1 (2019-12-17).
#   - This is the likely the cause for the google/protobuf gem not
#   loading correctly at runtime
#
# 2020-01-26
#   - Still the same, the workaround causes ruby to raise an issue
#   claiming that the google-protobuf module cannot be found
#   - If those workaround lines are commented out, we still get the
#   segfault as described here:
#       - https://github.com/protocolbuffers/protobuf/issues/4460
#
# 2020-09-11
#   (...)
#   "`require': cannot load such file -- google/2.7/protobuf_c
#   (LoadError)"
#   (...)
#   "Error relocating
#   /usr/local/bundle/gems/google-protobuf-3.13.0/lib/google/protobuf_c.so:
#   __va_copy: symbol not found -
#   /usr/local/bundle/gems/google-protobuf-3.13.0/lib/google/protobuf_c.so
#   (LoadError)"
#   - Possibly related:
#     https://github.com/protocolbuffers/protobuf/issues/3509#issuecomment-568050411
#

# stage: bootstrap-image
#
FROM ruby:2.7-alpine

WORKDIR /app

COPY ruby/Gemfile.lock .
COPY ruby/Gemfile .

COPY ruby/lib/ ./lib/
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
    && ./proto/bin/bootstrap -p proto/ -l ruby -d lib/ \
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
