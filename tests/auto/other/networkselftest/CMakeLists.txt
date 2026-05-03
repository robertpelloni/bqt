# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_networkselftest Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_networkselftest LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_networkselftest
    SOURCES
        tst_networkselftest.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Network
    BOBUI_TEST_SERVER_LIST "danted" "squid" "apache2" "ftp-proxy" "vsftpd" "cyrus" "echo"
)
