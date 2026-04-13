# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## chip Binary:
#####################################################################

bobui_internal_add_benchmark(chip
    GUI
    SOURCES
        chip.cpp chip.h
        main.cpp
        mainwindow.cpp mainwindow.h
        view.cpp view.h
    LIBRARIES
        BobUI::Gui
)

# Resources:
set(images_resource_files
    "fileprint.png"
    "bobui4logo.png"
    "rotateleft.png"
    "rotateright.png"
    "zoomin.png"
    "zoomout.png"
)

bobui_internal_add_resource(chip "images"
    PREFIX
        "/"
    FILES
        ${images_resource_files}
)

## Scopes:
#####################################################################

bobui_internal_extend_target(chip CONDITION TARGET BobUI::OpenGL
    LIBRARIES
        BobUI::OpenGL
)
