# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## cmptest Binary:
#####################################################################

bobui_internal_add_executable(cmptest
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_cmptest.cpp
    LIBRARIES
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_extend_target(cmptest CONDITION TARGET BobUI::Gui
    LIBRARIES
        BobUI::Gui
)

bobui_internal_apply_testlib_coverage_options(cmptest)
