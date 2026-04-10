# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(tex1d
    GUI
    SOURCES
        tex1d.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

set(tex1d_resource_files
    "texture1d.vert.qsb"
    "texture1d.frag.qsb"
)

bobui_internal_add_resource(tex1d "tex1d"
    PREFIX
        "/"
    FILES
        ${tex1d_resource_files}
)
