# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## keypadnavigation Binary:
#####################################################################

bobui_internal_add_manual_test(keypadnavigation
    GUI
    SOURCES
        keypadnavigation.ui
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)
