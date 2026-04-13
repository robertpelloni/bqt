# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## dialogs Binary:
#####################################################################

bobui_internal_add_manual_test(dialogs
    GUI
    SOURCES
        colordialogpanel.cpp colordialogpanel.h
        filedialogpanel.cpp filedialogpanel.h
        fontdialogpanel.cpp fontdialogpanel.h
        main.cpp
        messageboxpanel.cpp messageboxpanel.h
        utils.cpp utils.h
        wizardpanel.cpp wizardpanel.h
    NO_PCH_SOURCES
        filedialogpanel.cpp # undef BOBUI_NO_FOREACH
        utils.cpp # undef BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)

## Scopes:
#####################################################################

bobui_internal_extend_target(dialogs CONDITION TARGET BobUI::PrintSupport
    LIBRARIES
        BobUI::PrintSupport
)

bobui_internal_extend_target(dialogs CONDITION NOT (BOBUI_FEATURE_printer EQUAL FALSE)
    SOURCES
        printdialogpanel.cpp printdialogpanel.h printdialogpanel.ui
    NO_PCH_SOURCES
        printdialogpanel.cpp # undef BOBUI_NO_FOREACH
    ENABLE_AUTOGEN_TOOLS
        uic
)
