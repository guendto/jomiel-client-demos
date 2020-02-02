# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2020 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# build_with_docker()
#   - Determine relative path to the root directory containing the
#   .proto files
#
#   - Docker images copy the ../proto/ dir to their working dir
#
#   - When this option is enabled, cmake will look for the .proto files
#   from "./proto" rather than "../proto"
#
#   Sets:
#       BOOTSTRAP_PROTO_DIR; pointing to the root .proto dir
#
function(build_with_docker)
    option(WITH_DOCKER "build in docker container" OFF)

    if(WITH_DOCKER)
        set(_path ".")
    else()
        set(_path "..")
    endif()

    set(BOOTSTRAP_PROTO_DIR
        "${PROJECT_SOURCE_DIR}/${_path}/proto"
        PARENT_SCOPE
    )
endfunction()

# vim: set ts=4 sw=4 tw=72 expandtab:
