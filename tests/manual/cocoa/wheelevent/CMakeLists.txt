# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## wheelevent Binary:
#####################################################################

bobui_internal_add_manual_test(wheelevent
    GUI
    SOURCES
        main.cpp
        window.cpp window.h
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
)
