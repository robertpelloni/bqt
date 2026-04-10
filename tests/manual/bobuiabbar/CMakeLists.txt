# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobuiabbar Binary:
#####################################################################

bobui_internal_add_manual_test(tst_manual_bobuiabbar
    GUI
    SOURCES
        main.cpp
        tabbarform.cpp tabbarform.h tabbarform.ui
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)
