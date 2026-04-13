# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tableview Binary:
#####################################################################

bobui_internal_add_manual_test(tableview
    GUI
    SOURCES
        ../shared/shared.h
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
