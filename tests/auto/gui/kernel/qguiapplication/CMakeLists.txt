# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qguiapplication Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qguiapplication LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if (WIN32)
    set(target_version "1.2.3.4")
else()
    set(target_version "1.2.3")
endif()

# Resources:
set(tst_qguiapplication_resource_files
    "icons/appicon.png"
    "icons/usericon.png"
)

bobui_internal_add_test(tst_qguiapplication
    VERSION ${target_version}
    SOURCES
        ../../../corelib/kernel/qcoreapplication/tst_qcoreapplication.cpp ../../../corelib/kernel/qcoreapplication/tst_qcoreapplication.h
        tst_qguiapplication.cpp
    DEFINES
        BOBUI_QGUIAPPLICATIONTEST=1
    INCLUDE_DIRECTORIES
        ../../../corelib/kernel/qcoreapplication
    LIBRARIES
        BobUI::CorePrivate
        BobUI::GuiPrivate
    TESTDATA ${tst_qguiapplication_resource_files}
    BUILTIN_TESTDATA
)

if (ANDROID)
    set_property(TARGET tst_qguiapplication PROPERTY BOBUI_ANDROID_VERSION_NAME ${target_version})
endif()
if (APPLE)
    set_property(TARGET tst_qguiapplication PROPERTY MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
    set_property(TARGET tst_qguiapplication PROPERTY PROPERTY MACOSX_BUNDLE TRUE)
endif()

bobui_internal_add_test(tst_static_qguiapplication
    SOURCES
        ../../../corelib/kernel/qcoreapplication/tst_static_qcoreapplication.cpp
        ../../../corelib/kernel/qcoreapplication/maybeshow.h
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
)

bobui_internal_add_executable(apphelper_gui
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    SOURCES
        ../../../corelib/kernel/qcoreapplication/apphelper.cpp
        ../../../corelib/kernel/qcoreapplication/maybeshow.h
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
)
set_target_properties(apphelper_gui PROPERTIES OUTPUT_NAME apphelper)

add_dependencies(tst_qguiapplication
    apphelper_gui
)

if(BOBUI_FEATURE_library)
    bobui_internal_add_cmake_library(apphelper_gui_plugin
        MODULE
        SOURCES
            ../../../corelib/kernel/qcoreapplication/apphelper_plugin.cpp
        NO_PCH_SOURCES
            ../../../corelib/kernel/qcoreapplication/apphelper_plugin.cpp
        LIBRARIES
            BobUI::Gui
    )
    bobui_autogen_tools_initial_setup(apphelper_gui_plugin)
    add_dependencies(apphelper_gui apphelper_gui_plugin)
endif()
