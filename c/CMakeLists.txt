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

project(demo C)
cmake_minimum_required(VERSION 3.13)

# Settings.
#
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES OFF)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(
  -Wall -Werror -march=native -O1 -ggdb3 -pipe
  -Wformat -Wformat-security
  --param=ssp-buffer-size=4
  -fstack-protector-strong
  -Werror=format-security
  -D_FORTIFY_SOURCE=2
  -Wpedantic
)

list(APPEND CMAKE_MODULE_PATH # So we can find BootstrapProtoGen.
    "${CMAKE_CURRENT_SOURCE_DIR}/../tools/cmake")

# Helpers.
#   - required by bootstrap_get_bindings (and friends)
#
include(CMakePrintHelpers)

# Protobuf.
#
include(BootstrapProtoGen)

bootstrap_gen_bindings(c ext/proto)
#cmake_print_variables(SRC_BOOTSTRAP_FILES)

# Source files.
#
set(SRC_DEMO_FILES
    src/demo/jomiel.c
    src/demo/main.c
)

# Include dirs.
#
include_directories(${CMAKE_CURRENT_BINARY_DIR}/ext/proto)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(src)

# Find local libraries using pkg-config(1).
#
find_package(PkgConfig)

# Find pkg modules.
#
pkg_check_modules(PROTOBUFC REQUIRED libprotobuf-c)
pkg_check_modules(ZEROMQ REQUIRED libzmq)
pkg_check_modules(CZMQ REQUIRED libczmq)

# Target.
#
add_executable(demo ${SRC_DEMO_FILES} ${SRC_BOOTSTRAP_FILES})

target_include_directories(demo PUBLIC
    ${PROTOBUFC_INCLUDE_DIRS}
    ${ZEROMQ_INCLUDE_DIRS}
    ${CZMQ_INCLUDE_DIRS}
)

target_link_directories(demo PUBLIC
    ${PROTOBUFC_LIBRARY_DIRS}
    ${ZEROMQ_LIBRARY_DIRS}
    ${CZMQ_LIBRARY_DIRS}
)

target_link_libraries(demo
    ${PROTOBUFC_LIBRARIES}
    ${ZEROMQ_LIBRARIES}
    ${CZMQ_LIBRARIES}
)

# vim: set ts=4 sw=4 tw=72 expandtab:
