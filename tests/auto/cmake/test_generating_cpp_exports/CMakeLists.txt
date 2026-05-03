# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

include(.cmake.conf)

project(TestGeneratingCppExports
        DESCRIPTION "Test of the generating of cpp exports"
        HOMEPAGE_URL "https://bobui.io/"
        LANGUAGES CXX C
        VERSION "${BOBUI_REPO_MODULE_VERSION}"
)

find_package(BobUI6 COMPONENTS Core BuildInternals Test CONFIG REQUIRED)
bobui_internal_project_setup()

bobui_build_repo_begin()

add_subdirectory(test_autogenerating_cpp_exports)
add_subdirectory(test_autogenerating_cpp_exports_custom_name)

bobui_build_repo_post_process()
bobui_build_repo_end()
