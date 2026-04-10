# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qopenglcontext Binary:
#####################################################################

bobui_internal_add_manual_test(qopenglcontext
    GUI
    SOURCES
        main.cpp
        qopenglcontextwindow.cpp qopenglcontextwindow.h
    LIBRARIES
        BobUI::EglSupportPrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::OpenGL
)
