# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

if(BOBUI_BUILD_STANDALONE_EXAMPLES)
    # Needed for early feature values, despite it being found later on in bobui_build_tests().
    # Needs to be find_package, not bobui_find_package, because bobui_find_package doesn't handle finding
    # component for a super-package that has already been found.
    find_package(BobUI6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS Core)

    # Modify the optimization flags specifically for bobuibase standalone examples. Other projects
    # are handled by bobui_enable_cmake_languages().
    bobui_internal_set_up_config_optimizations_like_in_qmake()
endif()

bobui_examples_build_begin(EXTERNAL_BUILD)

add_compile_definitions(BOBUI_NO_CONTEXTLESS_CONNECT)

add_subdirectory(corelib)
if(TARGET BobUI6::DBus)
    add_subdirectory(dbus)
endif()
if(TARGET BobUI6::Network)
    add_subdirectory(network)
endif()
if(TARGET BobUI6::Test)
    add_subdirectory(bobuiestlib)
endif()
if(TARGET BobUI6::Concurrent)
    add_subdirectory(bobuiconcurrent)
endif()
if(TARGET BobUI6::Sql)
    add_subdirectory(sql)
endif()
if(TARGET BobUI6::Widgets)
    add_subdirectory(widgets)
endif()
if(TARGET BobUI6::Xml)
    add_subdirectory(xml)
endif()
if(TARGET BobUI6::Gui)
    add_subdirectory(gui)
endif()
if(BOBUI_FEATURE_opengl AND TARGET BobUI6::Gui)
    add_subdirectory(opengl)
endif()
if(BOBUI_FEATURE_vulkan AND TARGET BobUI6::Gui)
    add_subdirectory(vulkan)
endif()

bobui_examples_build_end()
