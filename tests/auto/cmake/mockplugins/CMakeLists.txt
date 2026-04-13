# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

include(.cmake.conf)
project(BobUIMockPlugins
    VERSION "${BOBUI_REPO_MODULE_VERSION}"
    DESCRIPTION "BobUI MockPlugins Libraries"
    HOMEPAGE_URL "https://bobui.io/"
    LANGUAGES CXX C
)

find_package(BobUI6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals CorePrivate)
bobui_internal_project_setup()

find_package(BobUI6 ${PROJECT_VERSION} QUIET CONFIG OPTIONAL_COMPONENTS Gui Widgets Xml)

bobui_build_repo_begin()

add_subdirectory(mockplugins1)
add_subdirectory(mockplugins2)
add_subdirectory(mockplugins3)

add_subdirectory(mock1plugin)
add_subdirectory(mock2plugin)
add_subdirectory(mock3plugin)
add_subdirectory(mock4plugin)
add_subdirectory(mock5plugin)
add_subdirectory(mock6plugin)

bobui_build_repo_post_process()
bobui_build_repo_end()
