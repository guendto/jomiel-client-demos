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

project(demo)
cmake_minimum_required(VERSION 3.13)

# Settings.
#
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES OFF)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14) # e.g. std::make_unique is a c++14 feature

add_compile_options(
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

if(WITH_DOCKER)
    include_directories(${PROJECT_SOURCE_DIR}/ext) # for cppzmq (alpine)
    set(PROTO_DIR "${PROJECT_SOURCE_DIR}/proto")
else()
    set(PROTO_DIR "${PROJECT_SOURCE_DIR}/../proto")
endif()

option(WITH_BOOTSTRAP_DEBUG "display details of the bootstrap script" OFF)

# Source files.
#
set(SRC_DEMO_FILES
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
set(DOCOPTCPP_TARGET        docoptcpp.git)
set(DOCOPTCPP_PREFIX        ${CMAKE_CURRENT_BINARY_DIR}/ext/${DOCOPTCPP_TARGET})
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
include_directories(${CMAKE_CURRENT_BINARY_DIR}/ext/proto)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(src)

# Find local libraries.
#   - kudos, https://stackoverflow.com/a/41252437 (ZeroMQ)
#
find_package(PkgConfig REQUIRED)
find_package(Protobuf REQUIRED)

pkg_check_modules(ZeroMQ REQUIRED libzmq)

# Protobuf
# - Compile the protobuf declarations for jomiel messages
#
set(BOOTSTRAP_COMMAND
    ${PROTO_DIR}/bin/bootstrap
    -p
    ${PROTO_DIR}
    -l
    cpp
    -d
    ${CMAKE_CURRENT_BINARY_DIR}/ext/proto
)

if(WITH_BOOTSTRAP_DEBUG)
    include(CMakePrintHelpers)
    cmake_print_variables(BOOTSTRAP_COMMAND)
    foreach(arg ${BOOTSTRAP_COMMAND})
        message(STATUS "  '${arg}'")
    endforeach()
endif()

message(STATUS "Compile the protobuf declarations for jomiel messages")

execute_process(COMMAND
    ${BOOTSTRAP_COMMAND}
    OUTPUT_QUIET
    RESULT_VARIABLE BOOTSTRAP_RESULT
    OUTPUT_VARIABLE BOOTSTRAP_OUTPUT
)

file(GLOB_RECURSE
    SRC_BOOTSTRAP_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/*.cc
    ${CMAKE_CURRENT_BINARY_DIR}/*.h
)

if(WITH_BOOTSTRAP_DEBUG)
    cmake_print_variables(SRC_BOOTSTRAP_FILES)
    cmake_print_variables(BOOTSTRAP_RESULT)
    cmake_print_variables(BOOTSTRAP_OUTPUT)
endif()

# Target.
#
add_executable(demo
    ${SRC_BOOTSTRAP_FILES}
    ${SRC_DEMO_FILES}
)

target_include_directories(demo PUBLIC
    ${DocOptCpp_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
    ${ZeroMQ_INCLUDE_DIRS}
)

target_link_directories(demo PUBLIC
    ${DocOptCpp_LIBRARY_DIRS}
    ${Protobuf_LIBRARY_DIRS}
    ${ZeroMQ_LIBRARY_DIRS}
)

target_link_libraries(demo PUBLIC
    ${Protobuf_LITE_LIBRARIES}
    ${DocOptCpp_LIBRARIES}
    ${Protobuf_LIBRARIES}
    ${ZeroMQ_LIBRARIES}
    libdocopt
)

# vim: set ts=4 sw=4 tw=72 expandtab:
