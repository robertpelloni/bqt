# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## childwindowcontainer Binary:
#####################################################################

bobui_internal_add_manual_test(childwindowcontainer
    GUI
    SOURCES
        main.cpp
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
