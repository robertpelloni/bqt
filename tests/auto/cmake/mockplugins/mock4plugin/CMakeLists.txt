# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QMock4Plugin
    NO_UNITY_BUILD
    CLASS_NAME QMock4Plugin
    PLUGIN_TYPE mockplugin
    DEFAULT_IF FALSE
    SOURCES
        qmock4plugin.cpp qmock4plugin.h
    LIBRARIES
        BobUI::Core
        BobUI::MockPlugins1
)
