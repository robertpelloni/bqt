# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(texturearray
    GUI
    SOURCES
        texturearray.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

set_source_files_properties("../shared/texture_arr.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture_arr.vert.qsb"
)
set_source_files_properties("../shared/texture_arr.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture_arr.frag.qsb"
)
set(texturearray_resource_files
    "../shared/texture_arr.vert.qsb"
    "../shared/texture_arr.frag.qsb"
)

bobui_internal_add_resource(texturearray "texturearray"
    PREFIX
        "/"
    FILES
        ${texturearray_resource_files}
)
