# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qgraphicsitemgroup Binary:
#####################################################################

bobui_internal_add_manual_test(qgraphicsitemgroup
    GUI
    SOURCES
        customitem.cpp customitem.h
        main.cpp
        widget.cpp widget.h widget.ui
    NO_PCH_SOURCES
        widget.cpp # undef BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)
