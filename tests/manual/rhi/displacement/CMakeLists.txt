# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(displacement
    GUI
    SOURCES
        displacement.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

bobui_internal_add_resource(displacement "displacement"
    PREFIX
        "/"
    FILES
        "material.vert.qsb"
        "material.tesc.qsb"
        "material.tese.qsb"
        "material.frag.qsb"
        "heightmap.png"
)

set(imgui_base ../shared/imgui)
set(imgui_target displacement)
include(${imgui_base}/imgui.cmakeinc)
