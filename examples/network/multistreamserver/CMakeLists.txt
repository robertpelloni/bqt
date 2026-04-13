# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(multistreamserver LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Network Widgets)

bobui_standard_project_setup()

bobui_add_executable(multistreamserver
    chatprovider.cpp chatprovider.h
    main.cpp
    movieprovider.cpp movieprovider.h
    provider.h
    server.cpp server.h
    timeprovider.cpp timeprovider.h
)

set_target_properties(multistreamserver PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(multistreamserver PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Network
    BobUI6::Widgets
)

install(TARGETS multistreamserver
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET multistreamserver
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
