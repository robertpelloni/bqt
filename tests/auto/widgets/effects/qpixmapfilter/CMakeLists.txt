# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qpixmapfilter Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qpixmapfilter LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qpixmapfilter
    SOURCES
        tst_qpixmapfilter.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
)
