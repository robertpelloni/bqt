# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## windowmodality Binary:
#####################################################################

bobui_internal_add_manual_test(windowmodality
    GUI
    SOURCES
        dialog.ui
        main.cpp
        widget.ui
    LIBRARIES
        BobUI::Gui
        BobUI::PrintSupport
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)
