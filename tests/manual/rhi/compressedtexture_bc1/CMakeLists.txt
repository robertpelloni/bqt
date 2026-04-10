# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## compressedtexture_bc1 Binary:
#####################################################################

bobui_internal_add_manual_test(compressedtexture_bc1
    GUI
    SOURCES
        compressedtexture_bc1.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

# Resources:
set_source_files_properties("../shared/bobui256_bc1_9mips.dds"
    PROPERTIES BOBUI_RESOURCE_ALIAS "bobui256_bc1_9mips.dds"
)
set_source_files_properties("../shared/texture.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.frag.qsb"
)
set_source_files_properties("../shared/texture.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.vert.qsb"
)
set(compressedtexture_bc1_resource_files
    "../shared/bobui256_bc1_9mips.dds"
    "../shared/texture.frag.qsb"
    "../shared/texture.vert.qsb"
)

bobui_internal_add_resource(compressedtexture_bc1 "compressedtexture_bc1"
    PREFIX
        "/"
    FILES
        ${compressedtexture_bc1_resource_files}
)
