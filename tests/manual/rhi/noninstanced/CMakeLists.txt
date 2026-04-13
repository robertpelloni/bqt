# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## noninstanced Binary:
#####################################################################

bobui_internal_add_manual_test(noninstanced
    GUI
    SOURCES
        noninstanced.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

# Resources:
set(noninstanced_resource_files
    "material.frag.qsb"
    "material.vert.qsb"
)

bobui_internal_add_resource(noninstanced "noninstanced"
    PREFIX
        "/"
    FILES
        ${noninstanced_resource_files}
)
