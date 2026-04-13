# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## stdinProcess Binary:
#####################################################################

bobui_internal_add_executable(stdinProcess
    INSTALL_DIRECTORY "${INSTALL_TESTSDIR}/tst_bobuiextstream/stdinProcess"
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    SOURCES
        main.cpp
)
