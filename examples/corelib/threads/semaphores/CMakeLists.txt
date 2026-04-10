# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(semaphores LANGUAGES CXX)

if (ANDROID)
    message(FATAL_ERROR "This project cannot be built on Android.")
endif()

find_package(BobUI6 REQUIRED COMPONENTS Core)

bobui_standard_project_setup()

bobui_add_executable(semaphores
    semaphores.cpp
)

target_link_libraries(semaphores PRIVATE
    BobUI6::Core
)

install(TARGETS semaphores
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET semaphores
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
