# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if (NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(iconbrowser LANGUAGES C CXX)
    find_package(BobUI6BuildInternals COMPONENTS STANDALONE_TEST)
endif()

find_package(BobUI6 REQUIRED COMPONENTS Gui Widgets)

bobui_internal_add_manual_test(iconbrowser
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
)

if (TARGET BobUI::Quick)
find_package(BobUI6 COMPONENTS QuickWidgets REQUIRED)

bobui_add_qml_module(iconbrowser
    URI main
    VERSION 1.0
    QML_FILES "Main.qml"
    NO_RESOURCE_TARGET_PATH
)
target_link_libraries(iconbrowser PRIVATE
    BobUI6::QuickWidgets
)
endif()

if (ANDROID)
    set(font_uri "MaterialSymbolsOutlined%5BFILL%2CGRAD%2Copsz%2Cwght%5D.ttf")
    set(font_filename "MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].ttf")
    if (BOBUI_ALLOW_DOWNLOAD)
        include(FetchContent)

        FetchContent_Declare(
            MaterialIcons
            URL
                "https://github.com/google/material-design-icons/raw/master/variablefont/${font_uri}"
            DOWNLOAD_DIR        ${CMAKE_CURRENT_BINARY_DIR}
            DOWNLOAD_NAME       "${font_filename}"
            DOWNLOAD_NO_EXTRACT TRUE
        )

        FetchContent_MakeAvailable(MaterialIcons)
    endif()

    if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${font_filename}")
        set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/${font_filename}"
                                    PROPERTIES BOBUI_RESOURCE_ALIAS ${font_filename})
        target_compile_definitions(iconbrowser PRIVATE "ICONBROWSER_RESOURCE")
        bobui_add_resources(iconbrowser "icons"
            PREFIX
                "/bobui-project.org/icons"
            FILES
                "${CMAKE_CURRENT_BINARY_DIR}/${font_filename}"
        )
    else()
        message(WARNING "Font file ${font_filename} not found and not downloaded!\n"
                        "Make sure the font file ${font_filename} is available in ${CMAKE_CURRENT_BINARY_DIR}.\n"
                        "Consider configuring with -DBOBUI_ALLOW_DOWNLOAD=ON to download the font automatically.")
    endif()
endif()

