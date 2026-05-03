# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(tst_stenciloutline
    GUI
    SOURCES
        stenciloutline.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

bobui_internal_add_resource(tst_stenciloutline "stenciloutline"
    PREFIX
        "/"
    FILES
        "material.frag.qsb"
        "material.vert.qsb"
)
