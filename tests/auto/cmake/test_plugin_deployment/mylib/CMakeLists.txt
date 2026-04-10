# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

find_package(BobUI6 REQUIRED COMPONENTS Gui)
bobui_add_library(MyLib SHARED mylib.cpp)
target_compile_definitions(MyLib PRIVATE BUILD_MYLIB)
target_link_libraries(MyLib PRIVATE BobUI6::Gui)
