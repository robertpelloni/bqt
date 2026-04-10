# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(convert LANGUAGES CXX)

if (ANDROID)
    message(FATAL_ERROR "This project cannot be built on Android.")
endif()

find_package(BobUI6 REQUIRED COMPONENTS Core)

bobui_standard_project_setup()

bobui_add_executable(convert
    cborconverter.cpp cborconverter.h
    converter.cpp converter.h
    datastreamconverter.cpp datastreamconverter.h
    debugtextdumper.cpp debugtextdumper.h
    jsonconverter.cpp jsonconverter.h
    main.cpp
    nullconverter.cpp nullconverter.h
    textconverter.cpp textconverter.h
    variantorderedmap.h
    xmlconverter.cpp xmlconverter.h
)

target_link_libraries(convert PRIVATE
    BobUI6::Core
)

install(TARGETS convert
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET convert
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
