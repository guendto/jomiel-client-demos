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
cmake_minimum_required(VERSION 3.13.4)
project(demo)

# Settings.
#
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES OFF)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

add_compile_options(
    -Wall -Werror -march=native -O1 -ggdb3 -pipe
    -Wformat -Wformat-security
    --param=ssp-buffer-size=4
    -fstack-protector-strong
    -Werror=format-security
    -D_FORTIFY_SOURCE=2
    -Wpedantic
)

# Helpers.
#   - Namely the useful cmake_print_variables() used everywhere.
#
include(CMakePrintHelpers)

# CMAKE_MODULE_PATH.
#   - Modify so that tools/cmake/*.cmake modules can be used.
#
list(APPEND CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/../tools/cmake)

# Protobuf.
#
include(BootstrapProtoGen)

bootstrap_gen_bindings(cpp build/gen)
#cmake_print_variables(SRC_BOOTSTRAP_FILES)

# Source files.
#
set(SRC_DEMO_FILES
    src/demo/appwindow.cpp
    src/demo/appwindow.h
    src/demo/mainwidget.cpp
    src/demo/mainwindow.h
    src/DemoAppWindow
    src/DemoMainWindow
    src/main.cpp
)

# Build local copies of external dependencies.
#
include(ExternalProject)

# nzmqt
#
set(NZMQT_TARGET        nzmqt)
set(NZMQT_PREFIX        ${CMAKE_CURRENT_BINARY_DIR}/ext/${NZMQT_TARGET})
set(Nzmqt_INCLUDE_DIRS  ${NZMQT_PREFIX}/src/nzmqt/include)
set(Nzmqt_LIBRARY_DIRS  ${NZMQT_PREFIX}/src/nzmqt/bin)
set(Nzmqt_LIBRARIES     ${NZMQT_PREFIX}/src/nzmqt/bin/libnzmqt.a)

set(NZMQT_PATCH_FILE
    ${PROJECT_SOURCE_DIR}/patches/0001-nzmqt.hpp-impl.hpp-fix-ensure-compilation-with-more-.patch)

externalproject_add(${NZMQT_TARGET}
    PREFIX          ${NZMQT_PREFIX}
    BINARY_DIR      ${NZMQT_PREFIX}/src/nzmqt # Working dir
    # ---
    GIT_REPOSITORY  https://github.com/jonnydee/nzmqt
    GIT_TAG         "master"
    # ---
    CONFIGURE_COMMAND qmake src/nzmqt_staticlib.pro
    BUILD_COMMAND     make
    INSTALL_COMMAND   ""
    # ---
    LOG_CONFIGURE   ON
    LOG_DOWNLOAD    ON
    LOG_INSTALL     ON
    LOG_BUILD       ON
    # ---
    UPDATE_DISCONNECTED True
    UPDATE_COMMAND      ""
    # ---
    PATCH_COMMAND  patch -p1 < ${NZMQT_PATCH_FILE}
)

add_library(libnzmqt STATIC IMPORTED)

set_target_properties(libnzmqt
    PROPERTIES IMPORTED_LOCATION ${Nzmqt_LIBRARIES})

add_dependencies(libnzmqt ${NZMQT_TARGET})

# Include dirs.
#
include_directories(${CMAKE_CURRENT_BINARY_DIR}/build/gen)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(src)

# Find pkgconfig and the library packages.
#   - kudos, https://stackoverflow.com/a/41252437 (ZeroMQ)
#
find_package(PkgConfig REQUIRED)
find_package(Protobuf REQUIRED)

pkg_check_modules(ZeroMQ REQUIRED libzmq)

# Qt.
#
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Target.
#
add_executable(demo
    ${SRC_BOOTSTRAP_FILES}
    ${SRC_DEMO_FILES}
)

target_include_directories(demo PUBLIC
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
    ${ZeroMQ_INCLUDE_DIRS}
    ${Nzmqt_INCLUDE_DIRS}
)

target_link_directories(demo PUBLIC
    ${Qt5Widgets_LIBRARY_DIRS}
    ${Protobuf_LIBRARY_DIRS}
    ${ZeroMQ_LIBRARY_DIRS}
    ${Nzmqt_LIBRARY_DIRS}
)

target_link_libraries(demo PUBLIC
    ${Qt5Widgets_LIBRARIES}
    ${Protobuf_LITE_LIBRARIES}
    ${Protobuf_LIBRARIES}
    ${ZeroMQ_LIBRARIES}
    ${Nzmqt_LIBRARIES}
    libnzmqt
)

# vim: set ts=4 sw=4 tw=72 expandtab:
