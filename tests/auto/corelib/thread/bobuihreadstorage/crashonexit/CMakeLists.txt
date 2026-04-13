# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## crashonexit Binary:
#####################################################################

bobui_internal_add_executable(crashOnExit_helper
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/.."
    INSTALL_DIRECTORY "${INSTALL_TESTSDIR}/tst_bobuihreadstorage/crashOnExit_helper"
    SOURCES
        crashOnExit.cpp
)
