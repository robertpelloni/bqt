# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## readLineStdinProcess Binary:
#####################################################################

bobui_internal_add_executable(readLineStdinProcess
    INSTALL_DIRECTORY "${INSTALL_TESTSDIR}/tst_bobuiextstream/readLineStdinProcess"
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    SOURCES
        main.cpp
)
