# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qpong Binary:
#####################################################################

bobui_internal_add_executable(qpong
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    SOURCES
        qpong.cpp
    LIBRARIES
        BobUI::DBus
)
