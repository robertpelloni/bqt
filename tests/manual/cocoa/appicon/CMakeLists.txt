# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## appicon Binary:
#####################################################################

bobui_internal_add_manual_test(appicon
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
