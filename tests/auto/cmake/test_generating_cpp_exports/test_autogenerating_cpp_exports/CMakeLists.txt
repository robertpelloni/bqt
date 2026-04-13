# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_module(TestAutogeneratingCppExports
    NO_UNITY_BUILD
    SOURCES
        module_api.h
        module_api.cpp
    PUBLIC_LIBRARIES
        BobUI::Core
)

bobui_internal_extend_target(TestAutogeneratingCppExports
    CONDITION GCC OR MINGW
    COMPILE_OPTIONS
        -fvisibility=hidden
)

add_executable(TestAutogeneratingCppExportsApp use_api.cpp)
target_link_libraries(TestAutogeneratingCppExportsApp PRIVATE TestAutogeneratingCppExports)
set_target_properties(TestAutogeneratingCppExportsApp PROPERTIES UNITY_BUILD OFF)
