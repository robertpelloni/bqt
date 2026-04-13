# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## mouse Binary:
#####################################################################

bobui_internal_add_executable(mouse
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_mouse.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Test
        BobUI::TestPrivate
)

## Scopes:
#####################################################################

bobui_internal_apply_testlib_coverage_options(mouse)
