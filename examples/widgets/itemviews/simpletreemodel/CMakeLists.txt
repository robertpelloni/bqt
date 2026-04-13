# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(simpletreemodel LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Test Widgets)

bobui_standard_project_setup()

bobui_add_executable(simpletreemodel
    main.cpp
    treeitem.cpp treeitem.h
    treemodel.cpp treemodel.h
)

set_target_properties(simpletreemodel PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(simpletreemodel PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(simpletreemodel_resource_files
    "default.txt"
)

bobui_add_resources(simpletreemodel "simpletreemodel"
    PREFIX
        "/"
    FILES
        ${simpletreemodel_resource_files}
)

install(TARGETS simpletreemodel
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET simpletreemodel
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

#! [1]
# Unit Test

include(CTest)

bobui_add_executable(simpletreemodel_tester
    test.cpp
    treeitem.cpp treeitem.h
    treemodel.cpp treemodel.h)

target_link_libraries(simpletreemodel_tester PRIVATE
    BobUI6::Core
    BobUI6::Test
)

if(ANDROID)
    target_link_libraries(simpletreemodel_tester PRIVATE
        BobUI6::Gui
    )
endif()

bobui_add_resources(simpletreemodel_tester "simpletreemodel_tester"
    PREFIX
        "/"
    FILES
        ${simpletreemodel_resource_files}
)

add_test(NAME simpletreemodel_tester
         COMMAND simpletreemodel_tester)
#! [1]
