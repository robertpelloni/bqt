# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## multiwindow Binary:
#####################################################################

bobui_internal_add_manual_test(multiwindow
    GUI
    SOURCES
        multiwindow.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
)

# Resources:
set_source_files_properties("../shared/color.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "color.frag.qsb"
)
set_source_files_properties("../shared/color.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "color.vert.qsb"
)
set(multiwindow_resource_files
    "../shared/color.frag.qsb"
    "../shared/color.vert.qsb"
)

bobui_internal_add_resource(multiwindow "multiwindow"
    PREFIX
        "/"
    FILES
        ${multiwindow_resource_files}
)
