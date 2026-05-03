# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## triangulator Binary:
#####################################################################

bobui_internal_add_manual_test(triangulator
    GUI
    SOURCES
        main.cpp
        triviswidget.cpp triviswidget.h
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
)
