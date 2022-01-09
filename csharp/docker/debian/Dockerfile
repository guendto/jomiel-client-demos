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

# In a nutshell
# - `paket` installs the deps from nuget.org
# - `premake5` generates the .sln file for `msbuild`
# - `msbuild` builds the project
# - `mkbundle` creates the self-contained executable
#

# stage: boostrap-image
#
FROM mono:6-slim AS boostrap-image

ENV WRKDIR /app

WORKDIR $WRKDIR

COPY csharp/paket.dependencies .
COPY csharp/paket.references .
COPY csharp/paket.lock .

COPY csharp/premake5.lua .

COPY csharp/src/ ./src/
COPY proto/ ./proto/

# URI to the latest release of `paket`.
#
ARG PKT_URI=https://api.github.com/repos/fsprojects/Paket/releases/latest

# github-api-release-v3 returns nothing for 'premake/premake-core'
# - Hardcoded URL will be used instead
#
ENV PM_VER 5.0.0-beta1
ENV PM_FILE premake-${PM_VER}-src.zip
ARG PM_URI=https://github.com/premake/premake-core/releases/download/v${PM_VER}/${PM_FILE}

RUN set -eux \
    # We want protobuf-compiler 3.12 from buster-backports.
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
    && ./proto/bin/bootstrap -p proto/ -l csharp -d src/proto/ \
    && mkdir -p ext \
        && cd ext \
        # Download premake5.
        && curl -sJLO ${PM_URI} \
        # Download paket.
        && curl -sJLO `curl -s ${PKT_URI} \
            | jq -r '.assets[] | select(.name == "paket.exe").browser_download_url'`

# stage: compile-ext-image
#
FROM boostrap-image AS compile-ext-image

ENV PATH "${WRKDIR}/ext/premake-${PM_VER}-src/bin/release:${PATH}"

RUN set -eux \
    # Build premake5.
    && cd ext \
        && unzip -q ${PM_FILE} \
        && cd premake-${PM_VER}-src/build/gmake2.unix \
        && make config=release Premake5 \
        && cd ${WRKDIR} \
    # Produce the .snl file.
    && premake5 --os=linux --dotnet=mono vs2019 \
    # Install the .net deps.
    && mono ext/paket.exe --silent install

# stage: compile-image
#
FROM compile-ext-image AS compile-image

RUN set -eux \
    ; msbuild build/demo.sln -noLogo -v:q -p:Configuration=release \
    && cd dist \
    && mkbundle -o demo --simple demo.exe --no-machine-config --no-config

# stage: runtime-image
#
FROM mono:6-slim AS runtime-image

COPY --from=compile-image /app/dist/demo /app/demo

RUN set -eux \
    && apt-get -qq update \
    && apt-get -qq --no-install-recommends install \
        libzmq5 \
    && rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["/app/demo"]

CMD ["-h"]
