# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qnetworkaccessmanager_and_qprogressdialog Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qnetworkaccessmanager_and_qprogressdialog LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qnetworkaccessmanager_and_qprogressdialog
    SOURCES
        tst_qnetworkaccessmanager_and_qprogressdialog.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Network
        BobUI::Widgets
    BOBUI_TEST_SERVER_LIST "apache2"
)
