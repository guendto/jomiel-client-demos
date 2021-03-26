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
cmake_minimum_required(VERSION 3.13)
project(demo C)

# Settings.
#
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES OFF)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_STANDARD 11)

add_compile_options(
    -Wall -Werror -march=native -O2 -ggdb3 -pipe
    -Wformat=2 -Wformat-security
    --param=ssp-buffer-size=4
    -fstack-protector-strong
    -Werror=format-security
    -D_FORTIFY_SOURCE=2
    -Wpedantic
    -Wmissing-include-dirs
    -Wredundant-decls
    -Wnested-externs
    -Wwrite-strings
    -Wshadow
    -Wextra
    -Wjump-misses-init -Wlogical-op
)

# Helpers.
#   - Namely the useful cmake_print_variables() used everywhere
#
include(CMakePrintHelpers)

# CMAKE_MODULE_PATH.
#   - Modify so that tools/cmake/*.cmake modules can be used.
#
list(APPEND CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/../tools/cmake)

# Protobuf.
#   - A "cmake-only" alternative to bootstrap script of jomiel-proto.
#
include(InvokeProtoc)

protoc_gen_bindings(c
    ${PROJECT_SOURCE_DIR}/../proto
    ${PROJECT_BINARY_DIR}/build/gen)
# cmake_print_variables(PROTOC_GEN_SRCFILES)

# Source files.
#
set(DEMO_SRCFILES
    src/demo/jomiel.c
    src/demo/main.c
)

# Include dirs.
#
include_directories(${PROJECT_BINARY_DIR}/build/gen)
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
add_executable(demo ${DEMO_SRCFILES} ${PROTOC_GEN_SRCFILES})

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
