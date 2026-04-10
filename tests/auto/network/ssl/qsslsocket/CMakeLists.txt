# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsslsocket LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT BOBUI_FEATURE_private_tests)
    return()
endif()

#####################################################################
## tst_qsslsocket Test:
#####################################################################

# Collect test data
list(APPEND test_data "certs")

bobui_internal_add_test(tst_qsslsocket
    SOURCES
        tst_qsslsocket.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::NetworkPrivate
        BobUI::TestPrivate
    TESTDATA ${test_data}
    BOBUI_TEST_SERVER_LIST "squid" "danted" "cyrus" "apache2" "echo"
    BUNDLE_ANDROID_OPENSSL_LIBS
)

bobui_internal_extend_target(tst_qsslsocket CONDITION BOBUI_FEATURE_private_tests AND BOBUI_FEATURE_openssl_linked
    LIBRARIES
        WrapOpenSSL::WrapOpenSSL
)
