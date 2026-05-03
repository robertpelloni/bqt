# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## modal Binary:
#####################################################################

bobui_internal_add_executable(modal_helper
    SOURCES
        base.cpp base.h
        main.cpp
    OUTPUT_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}/..
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)

## Scopes:
#####################################################################
