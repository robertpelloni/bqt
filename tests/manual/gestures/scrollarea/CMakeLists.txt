# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## scrollarea Binary:
#####################################################################

bobui_internal_add_manual_test(scrollarea
    GUI
    SOURCES
        main.cpp
        mousepangesturerecognizer.cpp mousepangesturerecognizer.h
    NO_PCH_SOURCES
        main.cpp # undef BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
