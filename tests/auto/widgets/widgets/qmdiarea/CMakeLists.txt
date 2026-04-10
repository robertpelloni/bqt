# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qmdiarea Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qmdiarea LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qmdiarea
    SOURCES
        tst_qmdiarea.cpp
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qmdiarea CONDITION TARGET BobUI::OpenGL
    LIBRARIES
        BobUI::OpenGL
)

bobui_internal_extend_target(tst_qmdiarea CONDITION APPLE
    LIBRARIES
        ${FWSecurity}
)
