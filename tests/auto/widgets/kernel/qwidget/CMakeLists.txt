# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qwidget Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qwidget LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Resources:
set(qwidget_resource_files
    "geometry-fullscreen.dat"
    "geometry-maximized.dat"
    "geometry.dat"
    "hellotr_la.qm"
)

bobui_internal_add_test(tst_qwidget
    SOURCES
        tst_qwidget.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::TestPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
    TESTDATA ${qwidget_resource_files}
    BUILTIN_TESTDATA
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qwidget CONDITION AIX
    COMPILE_OPTIONS
        -fpermissive
)

bobui_internal_extend_target(tst_qwidget CONDITION WIN32
    LIBRARIES
        gdi32
        user32
)
