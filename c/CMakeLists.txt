# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2019 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

project(demo C)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Werror -march=native -O1 -ggdb3 -pipe ")
string(APPEND CMAKE_C_FLAGS "-Wformat -Wformat-security ")
string(APPEND CMAKE_C_FLAGS "-fstack-protector-strong ")
string(APPEND CMAKE_C_FLAGS "--param=ssp-buffer-size=4 ")
string(APPEND CMAKE_C_FLAGS "-Werror=format-security ")
string(APPEND CMAKE_C_FLAGS "-D_FORTIFY_SOURCE=2 ")
string(APPEND CMAKE_C_FLAGS "-Wpedantic")

set(SRC_DEMO
    src/demo/proto/Message.pb-c.c
    src/demo/proto/Message.pb-c.h
    src/demo/proto/Status.pb-c.c
    src/demo/proto/Status.pb-c.h
    src/demo/proto/Media.pb-c.c
    src/demo/proto/Media.pb-c.h
    src/demo/jomiel.c
    src/demo/main.c
)

# Include dirs.
#
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
add_executable(demo ${SRC_DEMO})

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
