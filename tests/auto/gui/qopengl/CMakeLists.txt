# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qopengl Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qopengl LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qopengl
    SOURCES
        tst_qopengl.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::OpenGL
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qopengl CONDITION LINUX AND BOBUI_FEATURE_xcb AND BOBUI_FEATURE_xcb_glx_plugin
    DEFINES
        USE_GLX
)
