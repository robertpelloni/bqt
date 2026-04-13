# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_manual_graphicsview Binary:
#####################################################################

bobui_internal_add_manual_test(tst_manual_graphicsview
    GUI
    SOURCES
        gestures.cpp gestures.h
        imageitem.cpp imageitem.h
        main.cpp
        mousepangesturerecognizer.cpp mousepangesturerecognizer.h
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
