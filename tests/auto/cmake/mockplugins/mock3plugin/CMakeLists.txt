# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QMock3Plugin
    NO_UNITY_BUILD
    CLASS_NAME QMock3Plugin
    PLUGIN_TYPE mockplugin
    SOURCES
        qmock3plugin.cpp qmock3plugin.h
    LIBRARIES
        BobUI::Core
        BobUI::MockPlugins1
)
