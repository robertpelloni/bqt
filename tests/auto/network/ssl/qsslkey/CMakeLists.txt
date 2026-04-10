# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qsslkey Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsslkey LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    keys/*)
list(APPEND test_data ${test_data_glob})
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    rsa-*.pem)
list(APPEND test_data ${test_data_glob})

bobui_internal_add_test(tst_qsslkey
    SOURCES
        tst_qsslkey.cpp
    LIBRARIES
        BobUI::Network
    TESTDATA ${test_data}
    BUNDLE_ANDROID_OPENSSL_LIBS
)

bobui_internal_extend_target(tst_qsslkey CONDITION BOBUI_FEATURE_developer_build AND BOBUI_FEATURE_openssl_linked
    LIBRARIES
        WrapOpenSSL::WrapOpenSSL
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qsslkey CONDITION BOBUI_FEATURE_developer_build
    LIBRARIES
        BobUI::CorePrivate
        BobUI::NetworkPrivate
)
