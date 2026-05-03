# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(multiview
    GUI
    SOURCES
        multiview.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

bobui_internal_add_resource(multiview "multiview"
    PREFIX
        "/"
    FILES
        "multiview.vert.qsb"
        "multiview.frag.qsb"
        "texture.vert.qsb"
        "texture.frag.qsb"
)
