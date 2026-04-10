# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(optimize LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(BobUI6 REQUIRED COMPONENTS Core)

bobui_add_executable(optimize
    main.cpp
)

target_link_libraries(optimize PUBLIC
    BobUI::Core
)
if(DEFINED ENV{LIB_FUZZING_ENGINE})
    target_link_libraries(optimize PRIVATE
        $ENV{LIB_FUZZING_ENGINE}
    )
else()
    target_link_libraries(optimize PRIVATE
        -fsanitize=fuzzer
    )
endif()
