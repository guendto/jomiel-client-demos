# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

cmake_minimum_required(VERSION 3.16.3)
project(demo)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Settings.
#
# Set CMAKE_CXX_STANDARD until "How to manage C++ standard
# [EXPERIMENTAL]" solidifies.
#   <https://docs.conan.io/en/latest/howtos/manage_cpp_standard.html>
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
