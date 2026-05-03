# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QMock6Plugin
    NO_UNITY_BUILD
    CLASS_NAME QMock6Plugin
    PLUGIN_TYPE mockauxplugin
    SOURCES
        qmock6plugin.cpp qmock6plugin.h
    LIBRARIES
        BobUI::Core
        BobUI::MockPlugins3
)
