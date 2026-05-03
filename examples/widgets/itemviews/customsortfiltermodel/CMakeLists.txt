# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(customsortfiltermodel LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(customsortfiltermodel
    filterwidget.cpp filterwidget.h
    main.cpp
    mysortfilterproxymodel.cpp mysortfilterproxymodel.h
    window.cpp window.h
)

set_target_properties(customsortfiltermodel PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(customsortfiltermodel PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(customsortfiltermodel_resource_files
    "images/find.png"
)

bobui_add_resources(customsortfiltermodel "customsortfiltermodel"
    PREFIX
        "/"
    FILES
        ${customsortfiltermodel_resource_files}
)

install(TARGETS customsortfiltermodel
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET customsortfiltermodel
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
