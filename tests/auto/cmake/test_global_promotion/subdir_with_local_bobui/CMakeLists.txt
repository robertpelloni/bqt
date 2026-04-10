# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

message(STATUS "Entered subdir_with_local_bobui subdirectory")

set(file_path "${CMAKE_CURRENT_BINARY_DIR}/lib.cpp")
file(GENERATE OUTPUT "${file_path}" CONTENT "int foo() { return 42; }")
add_library(lib_local_bobui STATIC "${file_path}")

# These BobUI targets will be local to this directory scope.
find_package(BobUI6 REQUIRED COMPONENTS Gui)

target_link_libraries(lib_local_bobui PRIVATE BobUI6::Gui)

message(STATUS "Exiting subdir_with_local_bobui subdirectory")
