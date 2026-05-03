# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## deleteLater Binary:
#####################################################################

bobui_internal_add_executable(deleteLater
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_deleteLater.cpp
    LIBRARIES
        BobUI::Test
)

bobui_internal_apply_testlib_coverage_options(deleteLater)
