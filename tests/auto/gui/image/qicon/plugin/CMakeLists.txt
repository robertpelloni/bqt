# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## TestIconEngine Plugin:
#####################################################################

bobui_add_plugin(TestIconPlugin
    STATIC
    OUTPUT_NAME bobuiesticonplugin
    PLUGIN_TYPE iconengines
    main.cpp
)

bobui_autogen_tools_initial_setup(TestIconPlugin)
target_link_libraries(TestIconPlugin PRIVATE BobUI::Core BobUI::Gui)
