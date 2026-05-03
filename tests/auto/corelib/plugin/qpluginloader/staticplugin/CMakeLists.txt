# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## staticplugin Generic Library:
#####################################################################

bobui_internal_add_cmake_library(staticplugin
    STATIC
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Core
    MOC_OPTIONS
        "-M"
        "ExtraMetaData=StaticPlugin"
        "-M"
        "ExtraMetaData=foo"
)

# TEMPLATE = "lib"

bobui_autogen_tools_initial_setup(staticplugin)

target_compile_definitions(staticplugin PRIVATE BOBUI_STATICPLUGIN)
