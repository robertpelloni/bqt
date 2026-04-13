# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(androidnotifier LANGUAGES CXX)

if(NOT ANDROID)
    message(FATAL_ERROR "Example only works on Android")
endif()

find_package(BobUI6 REQUIRED COMPONENTS Widgets CorePrivate)

bobui_standard_project_setup()

bobui_add_executable(androidnotifier
    MANUAL_FINALIZATION
    main.cpp
    notificationclient.cpp
    notificationclient.h
    android/src/org/bobuiproject/example/androidnotifier/NotificationClient.java
    android/AndroidManifest.xml
)

target_link_libraries(androidnotifier PRIVATE
    BobUI6::CorePrivate
    BobUI6::Widgets
)

set_property(TARGET androidnotifier APPEND PROPERTY BOBUI_ANDROID_PACKAGE_SOURCE_DIR
             ${CMAKE_CURRENT_SOURCE_DIR}/android)

bobui_finalize_executable(androidnotifier)

set(qml_resource_files
    "images/happy.png"
    "images/sad.png"
)

bobui_add_resources(androidnotifier "main"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)

install(TARGETS androidnotifier
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET androidnotifier
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
