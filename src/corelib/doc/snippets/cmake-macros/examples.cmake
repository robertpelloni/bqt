# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#! [bobui_wrap_cpp_1]
set(SOURCES myapp.cpp main.cpp)
bobui_wrap_cpp(SOURCES myapp.h)
bobui_add_executable(myapp ${SOURCES})
#! [bobui_wrap_cpp_1]

#! [bobui_wrap_cpp_2]
set(SOURCES myapp.cpp main.cpp)
bobui_wrap_cpp(SOURCES myapp.h
            TARGET myapp
            OPTIONS
            "$<$<CONFIG:Debug>:-DMY_OPTION_FOR_DEBUG>"
            "-DDEFINE_CMDLINE_SIGNAL=void cmdlineSignal(const QMap<int, int> &i)"
            "$<$<CONFIG:Debug>:-DDEFINE_CMDLINE_SIGNAL_IN_GENEX=void cmdlineSignal(const QMap<int$<COMMA> int$<ANGLE-R> &i)>")
bobui_add_executable(myapp ${SOURCES})
#! [bobui_wrap_cpp_2]

#! [bobui_wrap_cpp_3]
set(SOURCES myapp.cpp main.cpp)
bobui_wrap_cpp(SOURCES myapp.h
            TARGET myapp)
bobui_add_executable(myapp ${SOURCES})
target_compile_definitions(myapp PRIVATE "$<$<CONFIG:Debug>:MY_OPTION_FOR_DEBUG>"
                                         "DEFINE_CMDLINE_SIGNAL=void cmdlineSignal(const QMap<int, int> &i)"
                                         "$<$<BOOL:TRUE>:DEFINE_CMDLINE_SIGNAL_IN_GENEX=void cmdlineSignal(const QMap<int$<COMMA> int$<ANGLE-R> &i)>")
#! [bobui_wrap_cpp_3]

#! [bobui_wrap_cpp_4]
bobui_add_executable(myapp myapp.cpp main.cpp)
bobui_wrap_cpp(myapp myapp.cpp)
#! [bobui_wrap_cpp_4]

#! [bobui_add_resources]
set(sources main.cpp)
bobui_add_resources(sources example.qrc)
bobui_add_executable(myapp ${sources})
#! [bobui_add_resources]

#! [bobui_add_resources_target]
bobui_add_executable(myapp main.cpp)
bobui_add_resources(myapp "images"
    PREFIX "/images"
    FILES image1.png image2.png)
#! [bobui_add_resources_target]

#! [bobui_add_big_resources]
set(SOURCES main.cpp)
bobui_add_big_resources(SOURCES big_resource.qrc)

# Have big_resource.qrc treated as a source file by BobUI Creator
list(APPEND SOURCES big_resource.qrc)
set_property(SOURCE big_resource.qrc PROPERTY SKIP_AUTORCC ON)

bobui_add_executable(myapp ${SOURCES})
#! [bobui_add_big_resources]

#! [bobui_add_binary_resources]
bobui_add_binary_resources(resources project.qrc OPTIONS -no-compress)
add_dependencies(myapp resources)
#! [bobui_add_binary_resources]

#! [bobui_generate_moc]
bobui_generate_moc(main.cpp main.moc TARGET myapp)
#! [bobui_generate_moc]

#! [bobui_import_plugins]
bobui_add_executable(myapp main.cpp)
target_link_libraries(myapp BobUI6::Gui BobUI6::Sql)
bobui_import_plugins(myapp
    INCLUDE BobUI6::QCocoaIntegrationPlugin
    EXCLUDE BobUI6::QMinimalIntegrationPlugin
    INCLUDE_BY_TYPE imageformats BobUI6::QGifPlugin BobUI6::QJpegPlugin
    EXCLUDE_BY_TYPE sqldrivers
)
#! [bobui_import_plugins]

#! [bobui_add_executable_simple]
bobui_add_executable(simpleapp main.cpp)
#! [bobui_add_executable_simple]

#! [bobui_add_executable_deferred]
bobui_add_executable(complexapp MANUAL_FINALIZATION complex.cpp)
set_target_properties(complexapp PROPERTIES OUTPUT_NAME Complexify)
bobui_finalize_target(complexapp)
#! [bobui_add_executable_deferred]

#! [bobui_android_deploy_basic]
bobui_android_generate_deployment_settings(myapp)
bobui_android_add_apk_target(myapp)
#! [bobui_android_deploy_basic]

#! [bobui_add_android_permission]
bobui_add_executable(myapp
    // ...
)
bobui_add_android_permission(myapp
    NAME android.permission.BLUETOOTH_SCAN
    ATTRIBUTES
        minSdkVersion 31
        usesPermissionFlags neverForLocation
)
bobui_add_android_permission(myapp
    NAME android.permission.ACCESS_COARSE_LOCATION
)
#! [bobui_add_android_permission]

#! [bobui_finalize_project_manual]
cmake_minimum_required(VERSIONS 3.16)

project(MyProject LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core)

bobui_add_executable(MyApp main.cpp)
add_subdirectory(mylib)

bobui_finalize_project()
#! [bobui_finalize_project_manual]

#! [AUTOGEN_BETTER_GRAPH_MULTI_CONFIG_1]
set(CMAKE_AUTOGEN_BETTER_GRAPH_MULTI_CONFIG ON)
#! [AUTOGEN_BETTER_GRAPH_MULTI_CONFIG_1]

#! [AUTOGEN_BETTER_GRAPH_MULTI_CONFIG_2]
set_target_properties(app PROPERTIES AUTOGEN_BETTER_GRAPH_MULTI_CONFIG ON)
#! [AUTOGEN_BETTER_GRAPH_MULTI_CONFIG_2]
