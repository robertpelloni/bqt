# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## generate Binary:
#####################################################################

bobui_internal_add_executable(generate
    GUI
    SOURCES
        main.cpp
    INCLUDE_DIRECTORIES
        /usr/include/freetype2
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
)
