# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## instancing Binary:
#####################################################################

bobui_internal_add_manual_test(tst_manual_instancing
    GUI
    SOURCES
        instancing.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

# Resources:
set(instancing_resource_files
    "inst.frag.qsb"
    "inst.vert.qsb"
)

bobui_internal_add_resource(tst_manual_instancing "instancing"
    PREFIX
        "/"
    FILES
        ${instancing_resource_files}
)
