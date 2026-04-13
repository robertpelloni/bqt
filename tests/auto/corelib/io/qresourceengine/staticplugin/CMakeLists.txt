# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## moctestplugin Generic Library:
#####################################################################

bobui_internal_add_cmake_library(moctestplugin
    STATIC
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Core
    DEFINES
        BOBUI_STATICPLUGIN
)

bobui_internal_add_resource(moctestplugin "qmake_plugin_resource"
    PREFIX
        "/staticplugin"
    FILES
        "main.cpp"
)

bobui_autogen_tools_initial_setup(moctestplugin)
