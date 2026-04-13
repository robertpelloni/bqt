# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## triquadcube Binary:
#####################################################################

bobui_internal_add_manual_test(triquadcube
    GUI
    SOURCES
        quadrenderer.cpp quadrenderer.h
        texturedcuberenderer.cpp texturedcuberenderer.h
        triangleoncuberenderer.cpp triangleoncuberenderer.h
        trianglerenderer.cpp trianglerenderer.h
        triquadcube.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

# Resources:
set_source_files_properties("../shared/color.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "color.frag.qsb"
)
set_source_files_properties("../shared/color.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "color.vert.qsb"
)
set_source_files_properties("../shared/bobui256.png"
    PROPERTIES BOBUI_RESOURCE_ALIAS "bobui256.png"
)
set_source_files_properties("../shared/texture.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.frag.qsb"
)
set_source_files_properties("../shared/texture.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.vert.qsb"
)
set(triquadcube_resource_files
    "../shared/color.frag.qsb"
    "../shared/color.vert.qsb"
    "../shared/bobui256.png"
    "../shared/texture.frag.qsb"
    "../shared/texture.vert.qsb"
)

bobui_internal_add_resource(triquadcube "triquadcube"
    PREFIX
        "/"
    FILES
        ${triquadcube_resource_files}
)
