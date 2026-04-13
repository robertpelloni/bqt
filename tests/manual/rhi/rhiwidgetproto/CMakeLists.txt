# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(rhiwidgetproto
    GUI
    SOURCES
        examplewidget.cpp examplewidget.h
        rhiwidget.cpp rhiwidget.h rhiwidget_p.h
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
)

set_source_files_properties("../shared/texture.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.vert.qsb"
)
set_source_files_properties("../shared/texture.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.frag.qsb"
)
set(rhiwidgetproto_resource_files
    "../shared/texture.vert.qsb"
    "../shared/texture.frag.qsb"
)

bobui_internal_add_resource(rhiwidgetproto "rhiwidgetproto"
    PREFIX
        "/"
    FILES
        ${rhiwidgetproto_resource_files}
)
