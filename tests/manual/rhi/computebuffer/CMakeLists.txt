# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## computebuffer Binary:
#####################################################################

bobui_internal_add_manual_test(computebuffer
    GUI
    SOURCES
        computebuffer.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

# Resources:
set(computebuffer_resource_files
    "buffer.comp.qsb"
    "main.frag.qsb"
    "main.vert.qsb"
)

bobui_internal_add_resource(computebuffer "computebuffer"
    PREFIX
        "/"
    FILES
        ${computebuffer_resource_files}
)


# TEMPLATE = "app"
