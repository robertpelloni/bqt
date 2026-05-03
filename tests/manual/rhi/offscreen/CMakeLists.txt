# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## offscreen Binary:
#####################################################################

bobui_internal_add_manual_test(offscreen
    SOURCES
        offscreen.cpp
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
set(offscreen_resource_files
    "../shared/color.frag.qsb"
    "../shared/color.vert.qsb"
)

bobui_internal_add_resource(offscreen "offscreen"
    PREFIX
        "/"
    FILES
        ${offscreen_resource_files}
)
