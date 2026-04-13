# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qclipboard Test:
#####################################################################

bobui_internal_add_test(tst_qclipboard
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../tst_qclipboard.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qclipboard CONDITION MACOS
    LIBRARIES
        ${FWAppKit}
)

# TEST_HELPER_INSTALLS = "../copier/copier" "../paster/paster"
