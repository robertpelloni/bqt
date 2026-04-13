# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## desktopsettingsaware Binary:
#####################################################################

bobui_internal_add_executable(desktopsettingsaware_helper
    SOURCES
        main.cpp
    OUTPUT_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}/..
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)

## Scopes:
#####################################################################
