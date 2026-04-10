# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#! [bobui_generate_deploy_script_example]
cmake_minimum_required(VERSION 3.16...3.22)
project(MyThings)

find_package(BobUI6 REQUIRED COMPONENTS Core)
bobui_standard_project_setup()

bobui_add_executable(MyApp main.cpp)

install(TARGETS MyApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

bobui_generate_deploy_script(
    TARGET MyApp
    OUTPUT_SCRIPT deploy_script
    CONTENT "
bobui_deploy_runtime_dependencies(
    EXECUTABLE $<TARGET_FILE:MyApp>
)
")
install(SCRIPT ${deploy_script})
#! [bobui_generate_deploy_script_example]
