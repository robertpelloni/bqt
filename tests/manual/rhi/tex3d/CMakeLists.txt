# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(tex3d
    GUI
    SOURCES
        tex3d.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

set(tex3d_resource_files
    "texture3d.vert.qsb"
    "texture3d.frag.qsb"
)

bobui_internal_add_resource(tex3d "tex3d"
    PREFIX
        "/"
    FILES
        ${tex3d_resource_files}
)
