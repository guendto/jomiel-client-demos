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

project(demo)
cmake_minimum_required(VERSION 3.13)

# Settings.
#
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES OFF)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
    # Disable deprecated declarations in cppzmq.
  -Wno-error=deprecated-declarations
  -Wall -Werror -march=native -O1 -ggdb3 -pipe
  -Wformat -Wformat-security
  --param=ssp-buffer-size=4
  -fstack-protector-strong
  -Werror=format-security
  -D_FORTIFY_SOURCE=2
  -Wpedantic
)

# Options.
#
option(WITH_DOCKER "build in docker container" OFF)

# CMAKE_MODULE_PATH.
#   - Modify so that tools/cmake/*.cmake modules can be used.
#
list(APPEND CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/../../tools/cmake)

# Helpers.
#   - Namely the useful cmake_print_variables() used everywhere.
#
include(CMakePrintHelpers)

# Protobuf.
#   - A "cmake-only" alternative to bootstrap script of jomiel-proto.
#
include(InvokeProtoc)

protoc_gen_bindings(cpp
    ${PROJECT_SOURCE_DIR}/../../proto
    ${PROJECT_BINARY_DIR}/build/gen)
# cmake_print_variables(PROTOC_GEN_SRCFILES)

# Source files.
#
set(SRC_DEMO_FILES
    ../src/demo/jomiel.cpp
    ../src/demo/jomiel.h
    ../src/demo/usage.cpp
    ../src/demo/main.cpp
)

# Build local copies of external dependencies.
#
include(ExternalProject)

## DocOpt.cpp
##  - kudos, https://stackoverflow.com/a/39112976
##
set(DOCOPTCPP_TARGET        docoptcpp.git)
set(DOCOPTCPP_PREFIX        ${CMAKE_BINARY_DIR}/ext/${DOCOPTCPP_TARGET})
set(DocOptCpp_INCLUDE_DIRS  ${DOCOPTCPP_PREFIX}/include)
set(DocOptCpp_LIBRARY_DIRS  ${DOCOPTCPP_PREFIX})
set(DocOptCpp_LIBRARIES     ${DOCOPTCPP_PREFIX}/libdocopt.a)

set(DOCOPTCPP_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DOCOPTCPP_PREFIX})

externalproject_add(${DOCOPTCPP_TARGET}
    CMAKE_ARGS      ${DOCOPTCPP_CMAKE_ARGS}
    PREFIX          ${DOCOPTCPP_PREFIX}
    # ---
    GIT_REPOSITORY  https://github.com/docopt/docopt.cpp
    GIT_TAG         "master"
    # ---
    # ---
    INSTALL_DIR     ${DOCOPTCPP_PREFIX}
    BINARY_DIR      ${DOCOPTCPP_PREFIX}
    # ---
    LOG_CONFIGURE   ON
    LOG_DOWNLOAD    ON
    LOG_INSTALL     ON
    LOG_BUILD       ON
    # ---
    UPDATE_DISCONNECTED True
    UPDATE_COMMAND      ""
)

add_library(libdocopt STATIC IMPORTED)

set_target_properties(libdocopt
    PROPERTIES IMPORTED_LOCATION ${DocOptCpp_LIBRARIES})

add_dependencies(libdocopt ${DOCOPTCPP_TARGET})

# Include dirs.
#
include_directories(${CMAKE_BINARY_DIR}/build/gen)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(../src)

if(WITH_DOCKER)
    # alpine: workaround alpine 3.10 not packaging cppzmq
    #   - download the missing zmq.hpp to the specified dir
    #   - include this dir
    include_directories(${CMAKE_BINARY_DIR}/../ext)
endif()

# Find local libraries.
#   - kudos, https://stackoverflow.com/a/41252437 (ZeroMQ)
#
find_package(PkgConfig REQUIRED)
find_package(Protobuf REQUIRED)

pkg_check_modules(ZeroMQ REQUIRED libzmq)
pkg_check_modules(Fmt REQUIRED fmt)

# Target.
#
add_executable(demo
    ${PROTOC_GEN_SRCFILES}
    ${SRC_DEMO_FILES}
)

target_include_directories(demo PUBLIC
    ${DocOptCpp_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
    ${ZeroMQ_INCLUDE_DIRS}
    ${Fmt_INCLUDE_DIRS}
)

target_link_directories(demo PUBLIC
    ${DocOptCpp_LIBRARY_DIRS}
    ${Protobuf_LIBRARY_DIRS}
    ${ZeroMQ_LIBRARY_DIRS}
    ${Fmt_LIBRARY_DIRS}
)

target_link_libraries(demo PUBLIC
    ${Protobuf_LITE_LIBRARIES}
    ${DocOptCpp_LIBRARIES}
    ${Protobuf_LIBRARIES}
    ${ZeroMQ_LIBRARIES}
    ${Fmt_LIBRARIES}
    libdocopt
)

# vim: set ts=4 sw=4 tw=72 expandtab:
