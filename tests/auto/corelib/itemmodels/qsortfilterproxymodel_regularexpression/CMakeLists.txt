# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qsortfilterproxymodel_regularexpression Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsfpm_regex LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qsfpm_regex
    SOURCES
        tst_qsortfilterproxymodel_regularexpression.cpp
    INCLUDE_DIRECTORIES
        ../../../other/qabstractitemmodelutils
    LIBRARIES
        BobUI::TestPrivate
)
