# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qopengltextureblitter Binary:
#####################################################################

bobui_internal_add_manual_test(qopengltextureblitter
    GUI
    SOURCES
        main.cpp
        qopengltextureblitwindow.cpp qopengltextureblitwindow.h
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::OpenGL
)
