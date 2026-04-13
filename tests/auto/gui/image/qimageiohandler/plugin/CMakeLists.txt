# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## TestImagePlugin:
#####################################################################

bobui_add_plugin(TestImagePlugin
    STATIC
    OUTPUT_NAME bobuiestimageplugin
    PLUGIN_TYPE imageformats
    main.cpp
    ../pluginlog.h
)

bobui_autogen_tools_initial_setup(TestImagePlugin)
target_link_libraries(TestImagePlugin PRIVATE BobUI::Core BobUI::Gui)
