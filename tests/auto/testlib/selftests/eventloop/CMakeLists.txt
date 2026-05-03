# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
#####################################################################
## eventloop Binary:
#####################################################################

bobui_internal_add_executable(eventloop
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_eventloop.cpp
    LIBRARIES
        BobUI::Test
)

## Scopes:
#####################################################################
bobui_internal_apply_testlib_coverage_options(eventloop)
