# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qasn1element Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qasn1element LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qasn1element
    SOURCES
        tst_qasn1element.cpp
        ../../../../../src/plugins/tls/shared/qasn1element_p.h
        ../../../../../src/plugins/tls/shared/qasn1element.cpp
    INCLUDE_DIRECTORIES
        ../../../../../src/plugins/tls/shared
    LIBRARIES
        BobUI::Core
        BobUI::Network
        BobUI::NetworkPrivate
    BUNDLE_ANDROID_OPENSSL_LIBS
)
