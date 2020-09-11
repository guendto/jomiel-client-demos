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

# stage: boostrap-image
#
FROM mono:6-slim AS boostrap-image

ENV WRKDIR /app

WORKDIR $WRKDIR

COPY csharp/paket.dependencies .
COPY csharp/log4net.config .

COPY csharp/premake5.lua .
COPY csharp/src/ ./src/

COPY proto/ ./proto/

# github-api-release-v3 returns nothing for 'premake/premake-core'
#   - Use a hardcoded URL for premake5 instead [2019-09-18]
#
# Debian packages premake4 but it is now old and apparently unmaintained
#   - We need to produce a modern .sln for msbuild (mono-devel)
#   - We aim to produce a standalone exe with mkbundle (mono-devel)
#   - The standalone exe still needs libzmq5 and mono-runtime packages
#

ENV PM_VER 5.0.0-alpha15
ENV PM_FILE premake-${PM_VER}-src.zip

ARG PM_URI=https://github.com/premake/premake-core/releases/download/v${PM_VER}/${PM_FILE}
ARG PKT_URI=https://api.github.com/repos/fsprojects/Paket/releases/latest

RUN set -eux \
    # We want to use protobuf-compiler 3.12 from buster-backports.
    #
    ; echo 'deb http://deb.debian.org/debian buster-backports main' \
        >> /etc/apt/sources.list \
    && apt-get -qq update \
    && apt-get -t buster-backports -qq --no-install-recommends install \
        protobuf-compiler \
        build-essential \
        mono-devel \
        msbuild \
        unzip \
        curl \
        jq \
        # NOTE:
        #  - libzmq5 package is not needed for building; install only
        #  until the mkbundle issue is resolved, see notes at the bottom
        #  of this file
        libzmq5 \
    && rm -rf /var/lib/apt/lists/* \
    && ./proto/bin/bootstrap -p proto/ -l csharp -d src/proto/ \
    && mkdir -p ext \
        && cd ext \
        # Download premake5.
        && curl -#JLO ${PM_URI} \
        # Download paket.
        && curl -#JLO `curl -s ${PKT_URI} \
            | jq -r '.assets[] | select(.name == "paket.exe").browser_download_url'`

# stage: compile-ext-image
#
FROM boostrap-image AS compile-ext-image

ENV PATH "${WRKDIR}/ext/premake-${PM_VER}/bin/release:${PATH}"

RUN set -eux \
    # Install the .net deps.
    ; mono ext/paket.exe --silent install \
    # Install premake5.
    && cd ext \
        && unzip ${PM_FILE} \
        && cd premake-${PM_VER}/build/gmake2.unix \
        && make config=release Premake5 \
        && cd ${WRKDIR} \
    # Produce the .snl file.
    && premake5 vs2019

# stage: compile-image
#
FROM compile-ext-image AS compile-image

RUN set -eux \
    ; msbuild demo.sln -v:q -p:Configuration=release
    # mkbundle: objects due to missing netstandard ref
    #   - like runtime-image below, comment out the mkbundle step
    #&& mkbundle -o demo --simple demo.exe \
    #    --machine-config /etc/mono/4.5/machine.config \
    #    --no-config

ENTRYPOINT ["mono", "/app/demo.exe"]

CMD ["-h"]

# stage: runtime-image
#   - commented out until mkbundle issue with netstandard is resolved
#
#FROM mono:6-slim AS runtime-image
#
#RUN set -eux \
#    ; apt-get -qq update \
#    && apt-get -qq --no-install-recommends install \
#        libzmq5 \
#    && rm -rf /var/lib/apt/lists/*
#
#COPY --from=compile-image /app/demo.exe.config /
#COPY --from=compile-image /app/demo /
#
#ENTRYPOINT ["/demo"]
#
#CMD ["-h"]
