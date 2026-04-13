# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## testSoftExit Binary:
#####################################################################

bobui_internal_add_executable(testSoftExit
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    CORE_LIBRARY None
)

bobui_internal_extend_target(testSoftExit CONDITION WIN32
    SOURCES
        main_win.cpp
    LIBRARIES
        user32
)

bobui_internal_extend_target(testSoftExit CONDITION UNIX
    SOURCES
        main_unix.cpp
)
