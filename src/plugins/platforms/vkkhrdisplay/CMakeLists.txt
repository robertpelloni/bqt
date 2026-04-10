# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_find_package(WrapFreetype MODULE PROVIDED_TARGETS WrapFreetype::WrapFreetype)

bobui_internal_add_plugin(QVkKhrDisplayIntegrationPlugin
    OUTPUT_NAME qvkkhrdisplay
    PLUGIN_TYPE platforms
    DEFAULT_IF "vkkhrdisplay" IN_LIST BOBUI_QPA_PLATFORMS
    SOURCES
        main.cpp
        qvkkhrdisplayintegration.cpp qvkkhrdisplayintegration.h
        qvkkhrdisplayvulkaninstance.cpp qvkkhrdisplayvulkaninstance.h
    DEFINES
        BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::FbSupportPrivate
        BobUI::Gui
        BobUI::GuiPrivate
)

bobui_internal_extend_target(QVkKhrDisplayIntegrationPlugin CONDITION BOBUI_FEATURE_freetype
    LIBRARIES
        WrapFreetype::WrapFreetype
)

bobui_internal_extend_target(QVkKhrDisplayIntegrationPlugin CONDITION TARGET BobUI::InputSupportPrivate
    LIBRARIES
        BobUI::InputSupportPrivate
)
