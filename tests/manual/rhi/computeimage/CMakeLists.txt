# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## computeimage Binary:
#####################################################################

bobui_internal_add_manual_test(computeimage
    GUI
    SOURCES
        computeimage.cpp
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
set(computeimage_resource_files
    "../shared/bobui256.png"
    "../shared/texture.frag.qsb"
    "../shared/texture.vert.qsb"
    "image.comp.qsb"
)

bobui_internal_add_resource(computeimage "computeimage"
    PREFIX
        "/"
    FILES
        ${computeimage_resource_files}
)
