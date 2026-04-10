# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## signaldumper Binary:
#####################################################################

bobui_internal_add_executable(signaldumper
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_signaldumper.cpp
    LIBRARIES
        BobUI::TestPrivate
)

## Scopes:
#####################################################################

bobui_internal_apply_testlib_coverage_options(signaldumper)
