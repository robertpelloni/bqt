# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## shadowmap Binary:
#####################################################################

bobui_internal_add_manual_test(shadowmap
    GUI
    SOURCES
        shadowmap.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

# Resources:
set(shadowmap_resource_files
    "main.frag.qsb"
    "main.vert.qsb"
    "shadowmap.frag.qsb"
    "shadowmap.vert.qsb"
)

bobui_internal_add_resource(shadowmap "shadowmap"
    PREFIX
        "/"
    FILES
        ${shadowmap_resource_files}
)
