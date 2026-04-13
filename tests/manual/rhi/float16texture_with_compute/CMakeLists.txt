# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## float16texture_with_compute Binary:
#####################################################################

bobui_internal_add_manual_test(float16texture_with_compute
    GUI
    SOURCES
        float16texture_with_compute.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

# Resources:
set_source_files_properties("../shared/bobui256.png"
    PROPERTIES BOBUI_RESOURCE_ALIAS "bobui256.png"
)
set_source_files_properties("../shared/texture.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.frag.qsb"
)
set_source_files_properties("../shared/texture.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.vert.qsb"
)
set(float16texture_with_compute_resource_files
    "../shared/bobui256.png"
    "../shared/texture.frag.qsb"
    "../shared/texture.vert.qsb"
    "load.comp.qsb"
    "prefilter.comp.qsb"
)

bobui_internal_add_resource(float16texture_with_compute "float16texture_with_compute"
    PREFIX
        "/"
    FILES
        ${float16texture_with_compute_resource_files}
)
