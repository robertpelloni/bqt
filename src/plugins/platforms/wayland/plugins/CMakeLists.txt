# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from plugins.pro.

add_subdirectory(hardwareintegration)
if(TARGET BobUI::WaylandClient)
    add_subdirectory(decorations)
    add_subdirectory(shellintegration)
endif()
