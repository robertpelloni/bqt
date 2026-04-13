# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## sleep Binary:
#####################################################################

bobui_internal_add_executable(sleep
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_sleep.cpp
    LIBRARIES
        BobUI::Test
        BobUI::CorePrivate
)

## Scopes:
#####################################################################

bobui_internal_apply_testlib_coverage_options(sleep)
