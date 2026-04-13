# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qjniobject Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qjniobject LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qjniobject
    SOURCES
        tst_qjniobject.cpp
    LIBRARIES
        BobUI::CorePrivate
)

# if we can, enable C++23 so that we can use std::expected in the test
# otherwise the test will use our own tl::expected copy from qexpected_p.h
if ("${CMAKE_CXX_COMPILE_FEATURES}" MATCHES "cxx_std_23")
    set_property(TARGET tst_qjniobject PROPERTY CXX_STANDARD 23)
endif()

if(ANDROID)
    set_property(TARGET tst_qjniobject APPEND PROPERTY BOBUI_ANDROID_PACKAGE_SOURCE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/testdata
    )
endif()
