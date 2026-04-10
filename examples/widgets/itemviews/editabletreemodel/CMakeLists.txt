# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(editabletreemodel LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Test Widgets)

bobui_standard_project_setup()

bobui_add_executable(editabletreemodel
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    treeitem.cpp treeitem.h
    treemodel.cpp treemodel.h
)

set_target_properties(editabletreemodel PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(editabletreemodel PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(editabletreemodel_resource_files
    "default.txt"
)

bobui_add_resources(editabletreemodel "editabletreemodel"
    PREFIX
        "/"
    FILES
        ${editabletreemodel_resource_files}
)

install(TARGETS editabletreemodel
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET editabletreemodel
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

#! [1]
# Unit Test

include(CTest)

bobui_add_executable(editabletreemodel_tester
    test.cpp
    treeitem.cpp treeitem.h
    treemodel.cpp treemodel.h)

target_link_libraries(editabletreemodel_tester PRIVATE
    BobUI6::Core
    BobUI6::Test
)

if(ANDROID)
    target_link_libraries(editabletreemodel_tester PRIVATE
        BobUI6::Gui
    )
endif()

bobui_add_resources(editabletreemodel_tester "editabletreemodel_tester"
    PREFIX
        "/"
    FILES
        ${editabletreemodel_resource_files}
)

add_test(NAME editabletreemodel_tester
         COMMAND editabletreemodel_tester)
#! [1]
