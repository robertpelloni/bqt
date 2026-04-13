# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## texuploads Binary:
#####################################################################

bobui_internal_add_manual_test(texuploads
    GUI
    SOURCES
        texuploads.cpp
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
set(texuploads_resource_files
    "../shared/bobui256.png"
    "../shared/texture.frag.qsb"
    "../shared/texture.vert.qsb"
)

bobui_internal_add_resource(texuploads "texuploads"
    PREFIX
        "/"
    FILES
        ${texuploads_resource_files}
)
