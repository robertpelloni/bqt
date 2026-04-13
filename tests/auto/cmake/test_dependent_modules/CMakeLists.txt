# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


cmake_minimum_required(VERSION 3.16)

project(test_dependent_modules)

# The module finds its dependencies
find_package(BobUI6Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

bobui_wrap_cpp(moc_files mywidget.h)
bobui_wrap_ui(ui_files mywidget.ui)

add_executable(mywidget mywidget.cpp ${moc_files} ${ui_files})
target_link_libraries(mywidget BobUI::Widgets)
