# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(polygonmode
    GUI
    SOURCES
        polygonmode.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

set(polygonmode_resource_files
    "test.vert.qsb"
    "test.frag.qsb"
)

bobui_internal_add_resource(polygonmode "polygonmode"
    PREFIX
        "/"
    FILES
        ${polygonmode_resource_files}
)
