# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(tessellation
    GUI
    SOURCES
        tessellation.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

set(tessellation_resource_files
    "test.vert.qsb"
    "test.tesc.qsb"
    "test.tese.qsb"
    "test.frag.qsb"
)

bobui_internal_add_resource(tessellation "tessellation"
    PREFIX
        "/"
    FILES
        ${tessellation_resource_files}
)
