# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## testtable1 Binary:
#####################################################################

bobui_internal_add_manual_test(testtable1
    GUI
    SOURCES
        testtable1.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
