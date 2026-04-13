# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
bobui_internal_add_manual_test(imguirenderer
    GUI
    SOURCES
        imguirenderer.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

set_source_files_properties("../shared/texture.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.vert.qsb"
)
set_source_files_properties("../shared/texture.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.frag.qsb"
)
set_source_files_properties("../shared/bobui256.png"
    PROPERTIES BOBUI_RESOURCE_ALIAS "bobui256.png"
)
bobui_internal_add_resource(imguirenderer "imguirenderer"
    PREFIX
        "/"
    FILES
        "../shared/texture.vert.qsb"
        "../shared/texture.frag.qsb"
        "../shared/bobui256.png"
)

set(imgui_base ../shared/imgui)
set(imgui_target imguirenderer)
include(${imgui_base}/imgui.cmakeinc)
