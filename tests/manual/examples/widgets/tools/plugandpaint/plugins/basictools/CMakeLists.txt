# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

bobui_add_plugin(pnp_basictools
    STATIC
    CLASS_NAME BasicToolsPlugin
    basictoolsplugin.cpp basictoolsplugin.h
)

target_include_directories(pnp_basictools PRIVATE
    ../../app
)

target_link_libraries(pnp_basictools PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)
