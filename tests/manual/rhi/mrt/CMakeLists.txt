# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## mrt Binary:
#####################################################################

bobui_internal_add_manual_test(mrt
    GUI
    SOURCES
        mrt.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

# Resources:
set_source_files_properties("../shared/texture.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.frag.qsb"
)
set_source_files_properties("../shared/texture.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.vert.qsb"
)
set(mrt_resource_files
    "../shared/texture.frag.qsb"
    "../shared/texture.vert.qsb"
    "mrt.frag.qsb"
    "mrt.vert.qsb"
)

bobui_internal_add_resource(mrt "mrt"
    PREFIX
        "/"
    FILES
        ${mrt_resource_files}
)
