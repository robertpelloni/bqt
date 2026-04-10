# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## test Test:
#####################################################################

# Collect test data
list(APPEND test_data "../tmp/README")
list(APPEND test_data "../modal")

bobui_internal_add_test(tst_qapplication
    SOURCES
        ../tst_qapplication.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
    TESTDATA ${test_data}
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/.."
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qapplication CONDITION builtin_testdata
    DEFINES
        BUILTIN_TESTDATA
)
