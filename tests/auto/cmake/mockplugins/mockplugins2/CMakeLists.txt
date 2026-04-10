# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_module(MockPlugins2
    NO_UNITY_BUILD
    SOURCES
        fake.cpp
    LIBRARIES
        BobUI::CorePrivate
    PUBLIC_LIBRARIES
        BobUI::Core
)
