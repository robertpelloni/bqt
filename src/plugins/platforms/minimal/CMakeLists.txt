# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QMinimalIntegrationPlugin Plugin:
#####################################################################

bobui_find_package(WrapFreetype MODULE PROVIDED_TARGETS WrapFreetype::WrapFreetype)

bobui_internal_add_plugin(QMinimalIntegrationPlugin
    OUTPUT_NAME qminimal
    PLUGIN_TYPE platforms
    DEFAULT_IF "minimal" IN_LIST BOBUI_QPA_PLATFORMS
    SOURCES
        main.cpp
        qminimalbackingstore.cpp qminimalbackingstore.h
        qminimalintegration.cpp qminimalintegration.h
    DEFINES
        BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QMinimalIntegrationPlugin CONDITION BOBUI_FEATURE_freetype
    LIBRARIES
        WrapFreetype::WrapFreetype
)
