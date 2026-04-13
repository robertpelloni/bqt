# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(remotecontrolledcar LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core DBus Gui Widgets)

bobui_standard_project_setup()

add_subdirectory(car)
add_subdirectory(controller)
