# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## multiwindow_threaded Binary:
#####################################################################

bobui_internal_add_manual_test(multiwindow_threaded
    GUI
    SOURCES
        multiwindow_threaded.cpp
        window.cpp window.h
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
)

# Resources:
set_source_files_properties("../shared/bobui256.png"
    PROPERTIES BOBUI_RESOURCE_ALIAS "bobui256.png"
)
set_source_files_properties("../shared/texture.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.frag.qsb"
)
set_source_files_properties("../shared/texture.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.vert.qsb"
)
set(multiwindow_threaded_resource_files
    "../shared/bobui256.png"
    "../shared/texture.frag.qsb"
    "../shared/texture.vert.qsb"
)

bobui_internal_add_resource(multiwindow_threaded "multiwindow_threaded"
    PREFIX
        "/"
    FILES
        ${multiwindow_threaded_resource_files}
)


# TEMPLATE = "app"
