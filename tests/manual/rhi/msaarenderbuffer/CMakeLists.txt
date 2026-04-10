# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## msaarenderbuffer Binary:
#####################################################################

bobui_internal_add_manual_test(msaarenderbuffer
    GUI
    SOURCES
        msaarenderbuffer.cpp
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
set_source_files_properties("../shared/texture.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.frag.qsb"
)
set_source_files_properties("../shared/texture.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.vert.qsb"
)
set(msaarenderbuffer_resource_files
    "../shared/color.frag.qsb"
    "../shared/color.vert.qsb"
    "../shared/texture.frag.qsb"
    "../shared/texture.vert.qsb"
)

bobui_internal_add_resource(msaarenderbuffer "msaarenderbuffer"
    PREFIX
        "/"
    FILES
        ${msaarenderbuffer_resource_files}
)
