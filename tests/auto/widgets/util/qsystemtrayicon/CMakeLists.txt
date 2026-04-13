# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qsystemtrayicon Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsystemtrayicon LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

set(resources_resource_files
    "icons/icon.png"
)

bobui_internal_add_test(tst_qsystemtrayicon
    SOURCES
        tst_qsystemtrayicon.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
    TESTDATA ${resources_resource_files}
    BUILTIN_TESTDATA
)
