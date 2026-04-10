# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## app Binary:
#####################################################################

bobui_internal_add_executable(app
    GUI
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
