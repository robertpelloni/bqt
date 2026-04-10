# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(geometryshader
    GUI
    SOURCES
        geometryshader.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

set(geometryshader_resource_files
    "test.vert.qsb"
    "test.geom.qsb"
    "test.frag.qsb"
)

bobui_internal_add_resource(geometryshader "geometryshader"
    PREFIX
        "/"
    FILES
        ${geometryshader_resource_files}
)
