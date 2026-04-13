# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

include(.cmake.conf)

project(TestStaticResources
        DESCRIPTION "Test of the static resources"
        HOMEPAGE_URL "https://bobui.io/"
        LANGUAGES CXX C
        VERSION "${BOBUI_REPO_MODULE_VERSION}"
)

find_package(BobUI6 COMPONENTS Core BuildInternals Test CONFIG REQUIRED)
bobui_internal_project_setup()

bobui_build_repo_begin()

enable_testing()
add_subdirectory(mock_static_resources1)
add_subdirectory(test_init_resources_static_plugin)
add_subdirectory(test_static_resources_propagation)

bobui_build_repo_post_process()
bobui_build_repo_end()
