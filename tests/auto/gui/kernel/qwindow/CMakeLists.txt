# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qwindow Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qwindow LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qwindow
    SOURCES
        tst_qwindow.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::TestPrivate
)

if(APPLE OR WIN32 OR BOBUI_FEATURE_xcb)
    bobui_internal_add_test(tst_foreignwindow
        LOWDPI
        SOURCES
            tst_foreignwindow.cpp
        LIBRARIES
            BobUI::CorePrivate
            BobUI::Gui
            BobUI::GuiPrivate
    )

    if(APPLE)
        enable_language(OBJCXX)
        set_source_files_properties(tst_foreignwindow.cpp PROPERTIES LANGUAGE OBJCXX)
        set_property(TARGET tst_foreignwindow PROPERTY PROPERTY MACOSX_BUNDLE TRUE)
    endif()

    if(BOBUI_FEATURE_xcb)
        target_link_libraries(tst_foreignwindow PRIVATE XCB::XCB)
    endif()
endif()

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qwindow CONDITION BOBUI_FEATURE_dynamicgl AND WIN32
    LIBRARIES
        user32
)
