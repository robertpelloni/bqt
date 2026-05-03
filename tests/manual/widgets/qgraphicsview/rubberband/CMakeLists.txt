# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## rubberband Binary:
#####################################################################

bobui_internal_add_manual_test(rubberband
    GUI
    SOURCES
        rubberbandtest.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
