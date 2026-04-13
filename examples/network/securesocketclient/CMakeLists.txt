# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(securesocketclient LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Network Widgets)

bobui_standard_project_setup()

bobui_add_executable(securesocketclient
    certificateinfo.cpp certificateinfo.h certificateinfo.ui
    main.cpp
    sslclient.cpp sslclient.h sslclient.ui
    sslerrors.ui
)

set_target_properties(securesocketclient PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(securesocketclient PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Network
    BobUI6::Widgets
)

# Resources:
set(securesocketclient_resource_files
    "encrypted.png"
)

bobui_add_resources(securesocketclient "securesocketclient"
    PREFIX
        "/"
    FILES
        ${securesocketclient_resource_files}
)

install(TARGETS securesocketclient
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET securesocketclient
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
