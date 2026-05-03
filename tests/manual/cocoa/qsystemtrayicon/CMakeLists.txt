# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qsystemtrayicon Binary:
#####################################################################

bobui_internal_add_manual_test(qsystemtrayicon
    GUI
    SOURCES
        main.cpp
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)

# Resources:
set(icons_resource_files
    "macsystray16x16.png"
    "macsystray18x18.png"
    "macsystray25x15.png"
    "macsystray32x32.png"
    "macsystray36x36.png"
    "macsystray50x30.png"
    "macsystray64x64.png"
)

bobui_internal_add_resource(qsystemtrayicon "icons"
    PREFIX
        "/"
    FILES
        ${icons_resource_files}
)
