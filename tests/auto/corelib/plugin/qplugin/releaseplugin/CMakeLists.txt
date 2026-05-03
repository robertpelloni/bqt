# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## releaseplugin Generic Library:
#####################################################################

bobui_internal_add_cmake_library(releaseplugin
    MODULE
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../plugins"
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Core
)

bobui_autogen_tools_initial_setup(releaseplugin)
