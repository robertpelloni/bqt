# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
bobui_internal_add_manual_test(hdr
    GUI
    SOURCES
        hdr.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

bobui_internal_add_resource(hdr "hdr"
    PREFIX
        "/"
    FILES
        "hdrtexture.vert.qsb"
        "hdrtexture.frag.qsb"
)

set(imgui_base ../shared/imgui)
set(imgui_target hdr)
include(${imgui_base}/imgui.cmakeinc)
