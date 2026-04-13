# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## deleteLater_noApp Binary:
#####################################################################

bobui_internal_add_executable(deleteLater_noApp
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_deleteLater_noApp.cpp
    LIBRARIES
        BobUI::Test
)

bobui_internal_apply_testlib_coverage_options(deleteLater_noApp)
