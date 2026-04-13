# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(rangemodel VERSION 0.1 LANGUAGES CXX)

find_package(BobUI6 COMPONENTS
    Core Gui Widgets
    REQUIRED
)

find_package(BobUI6 OPTIONAL_COMPONENTS
    Quick
    QuickWidgets
)

if (TARGET BobUI6::Quick)
    bobui_policy(SET BOBUIP0001 NEW)
endif()

bobui_standard_project_setup()

bobui_add_executable(rangemodel
    MANUAL_FINALIZATION
    main.cpp
)

target_compile_options(rangemodel PRIVATE -fsanitize=address)
target_link_options(rangemodel PRIVATE -fsanitize=address)

target_link_libraries(rangemodel PRIVATE
    BobUI6::Widgets
)

if (TARGET BobUI6::Quick)
    target_link_libraries(rangemodel PRIVATE
       BobUI6::Quick
       BobUI6::QuickWidgets
    )
endif()

set_property(TARGET rangemodel APPEND PROPERTY
             BOBUI_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
)



bobui_internal_extend_target(rangemodel CONDITION MSVC
    COMPILE_OPTIONS
        /bigobj
)

if ("${CMAKE_CXX_COMPILE_FEATURES}" MATCHES "cxx_std_23")
    set_property(TARGET rangemodel PROPERTY CXX_STANDARD 23)
endif()

bobui_finalize_executable(rangemodel)

if (TARGET BobUI6::Quick)
    bobui_add_qml_module(rangemodel
        URI Main
        VERSION 1.0
        QML_FILES "Main.qml"
        NO_RESOURCE_TARGET_PATH
    )
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
