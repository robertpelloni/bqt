# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qsortfilterproxymodel Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsortfilterproxymodel LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qsortfilterproxymodel
    SOURCES
        ../../../other/qabstractitemmodelutils/dynamictreemodel.cpp ../../../other/qabstractitemmodelutils/dynamictreemodel.h
        tst_qsortfilterproxymodel.cpp
    INCLUDE_DIRECTORIES
        ../../../other/qabstractitemmodelutils
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
        BobUI::TestPrivate
)
