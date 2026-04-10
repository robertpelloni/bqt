# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QMock2Plugin
    NO_UNITY_BUILD
    CLASS_NAME QMock2Plugin
    PLUGIN_TYPE mockplugin
    SOURCES
        qmock2plugin.cpp qmock2plugin.h
    LIBRARIES
        BobUI::Core
        BobUI::MockPlugins1
)
