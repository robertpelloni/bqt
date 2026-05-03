# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(loongarch_simd LANGUAGES CXX)

include(../../cmake/BobUIPlatformSupport.cmake)
include(../../cmake/BobUICompilerOptimization.cmake)

# FIXME: Make the this project handle a list of SIMD entries.
# FIXME: Make this project handle appending of the cflags (similar to the qmake project).
# This is needed for the loongarchsimd configure test (
# aka we test to see if setting no SIMD (-mlsx) cflags at all, will result in their implicit
# addition by the compiler).
string(TOUPPER "${SIMD}" upper_simd)

if(NOT DEFINED "BOBUI_CFLAGS_${upper_simd}")
    # Don't use CMake error() because a configure error also fails the try_compile() call.
    # Instead use a compile flag that doesn't exist to force a compiler error.
    set(BOBUI_CFLAGS_${upper_simd} "--bobui-cflags-not-found")
endif()

add_executable("SimdTest${SIMD}")
target_sources("SimdTest${SIMD}" PRIVATE main.cpp)
target_compile_options("SimdTest${SIMD}" PRIVATE ${BOBUI_CFLAGS_${upper_simd}})
target_compile_definitions("SimdTest${SIMD}" PRIVATE BOBUI_COMPILER_SUPPORTS_${upper_simd})
