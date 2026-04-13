# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


find_package(BobUI6Core REQUIRED)

add_executable(exe2 "${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp")
target_link_libraries(exe2 BobUI::Core)
