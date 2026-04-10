# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(icons LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/widgets/icons")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(icons
    iconpreviewarea.cpp iconpreviewarea.h
    iconsizespinbox.cpp iconsizespinbox.h
    imagedelegate.cpp imagedelegate.h
    main.cpp
    mainwindow.cpp mainwindow.h
)

set_target_properties(icons PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_compile_definitions(icons PRIVATE
    SRCDIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(icons PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

install(TARGETS icons
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
