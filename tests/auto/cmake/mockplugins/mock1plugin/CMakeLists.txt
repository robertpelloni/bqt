# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QMock1Plugin
    NO_UNITY_BUILD
    CLASS_NAME QMock1Plugin
    PLUGIN_TYPE mockplugin
    SOURCES
        qmock1plugin.cpp qmock1plugin.h
    LIBRARIES
        BobUI::Core
        BobUI::MockPlugins1
)
