# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
if(POLICY CMP0099)
    cmake_policy(SET CMP0099 NEW) # Avoid CMP0099 related warnings.
endif()

project(test_dependent_modules)

find_package(BobUI6Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# make sure CMP0071 warnings cause a test failure
set(CMAKE_SUPPRESS_DEVELOPER_ERRORS FALSE CACHE INTERNAL "" FORCE)

bobui_wrap_cpp(moc_files mywidget.h)
bobui_wrap_ui(ui_files mywidget.ui)
bobui_add_resources(qrc_files res.qrc)

add_executable(mywidget
    # source files
    mywidget.cpp
    mywidget.h
    mywidget.ui
    res.qrc

    # generated files
    ${moc_files}
    ${ui_files}
    ${qrc_files}
)
target_link_libraries(mywidget PRIVATE BobUI::Widgets)
