# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsslsocket_onDemandCertificates_static LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT BOBUI_FEATURE_private_tests)
    return()
endif()

#####################################################################
## tst_qsslsocket_onDemandCertificates_static Test:
#####################################################################

bobui_internal_add_test(tst_qsslsocket_onDemandCertificates_static
    SOURCES
        tst_qsslsocket_onDemandCertificates_static.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::NetworkPrivate
    BOBUI_TEST_SERVER_LIST "squid" "danted"
    BUNDLE_ANDROID_OPENSSL_LIBS
)
