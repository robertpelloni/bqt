# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## invalidplugin Generic Library:
#####################################################################

bobui_internal_add_cmake_library(invalidplugin
    MODULE
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../plugins"
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Core
)

# TEMPLATE = "lib"

bobui_autogen_tools_initial_setup(invalidplugin)
