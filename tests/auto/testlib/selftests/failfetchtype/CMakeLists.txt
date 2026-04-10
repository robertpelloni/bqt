# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## failfetchtype Binary:
#####################################################################

bobui_internal_add_executable(failfetchtype
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_failfetchtype.cpp
    LIBRARIES
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_apply_testlib_coverage_options(failfetchtype)
