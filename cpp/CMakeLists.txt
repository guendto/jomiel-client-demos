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

project(demo)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 14)
  # e.g. std::make_unique is a c++14 feature

# Additional CXX flags of interest.
#
#set(CMAKE_CXX_FLAGS "-Wall -Werror -march=native -O1 -ggdb3 -pipe ")
#string(APPEND CMAKE_CXX_FLAGS "-Wformat -Wformat-security ")
#string(APPEND CMAKE_CXX_FLAGS "-fstack-protector-strong ")
#string(APPEND CMAKE_CXX_FLAGS "--param=ssp-buffer-size=4 ")
#string(APPEND CMAKE_CXX_FLAGS "-Werror=format-security ")
#string(APPEND CMAKE_CXX_FLAGS "-D_FORTIFY_SOURCE=2 ")
#string(APPEND CMAKE_CXX_FLAGS "-Wpedantic ")

# Source files.
#
set(SRC_DEMO
    src/demo/jomiel.cpp
    src/demo/jomiel.h
    src/demo/usage.cpp
    src/demo/main.cpp
    src/demo/types.h
)

# Build local copies of external dependencies.
#
include(ExternalProject)

## DocOpt.cpp
##  - kudos, https://stackoverflow.com/a/39112976
##
set(DOCOPT_ROOT ${PROJECT_SOURCE_DIR}/ext/docopt)
set(DOCOPT_INCLUDE_DIRS ${DOCOPT_ROOT}/include)
set(DOCOPT_LIBRARIES ${DOCOPT_ROOT}/lib/libdocopt.a)
set(DOCOPT_INSTALL_DIR "${DOCOPT_ROOT}")
set(DOCOPT_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DOCOPT_INSTALL_DIR})

ExternalProject_Add(docopt
    PREFIX ${DOCOPT_ROOT}
    # ---
    GIT_REPOSITORY https://github.com/docopt/docopt.cpp
    GIT_TAG "master"
    UPDATE_DISCONNECTED True
    CMAKE_ARGS ${DOCOPT_CMAKE_ARGS}
    INSTALL_DIR ${DOCOPT_ROOT}
    BINARY_DIR ${DOCOPT_ROOT}
    # ---
    LOG_CONFIGURE ON
    LOG_DOWNLOAD ON
    LOG_INSTALL ON
    LOG_BUILD ON
)

add_library(libdocopt STATIC IMPORTED)
add_dependencies(libdocopt docopt)

set_target_properties(libdocopt
    PROPERTIES IMPORTED_LOCATION ${DOCOPT_LIBRARIES})

# Include dirs.
#
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${DOCOPT_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(src)

# Protobuf
# - Find the lib, compile the protobuf declarations for jomiel messages
#
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

file(GLOB PROTO_FILES ../proto/*.proto)
protobuf_generate_cpp(SRC_PROTO HDR_PROTO ${PROTO_FILES})

# ZeroMQ.
#   - kudos, https://stackoverflow.com/a/41252437
#
find_package(PkgConfig)

pkg_check_modules(ZeroMQ libzmq)

find_path(ZeroMQ_INCLUDE_DIR
    PATHS ${ZeroMQ_INCLUDE_DIRS}
    NAMES zmq.hpp
)

find_library(ZeroMQ_LIBRARY
    PATHS ${ZeroMQ_LIBRARY_DIRS}
    NAMES zmq
)

# Target.
#
add_executable(demo ${SRC_DEMO} ${SRC_PROTO} ${HDR_PROTO} ${docopt_LIBRARY})

target_include_directories(demo PUBLIC
    ${ZeroMQ_INCLUDE_DIR}
)

target_link_libraries(demo
    ${Protobuf_LIBRARIES}
    ${ZeroMQ_LIBRARY}
    libdocopt
    zmq
)

# vim: set ts=4 sw=4 tw=72 expandtab:
