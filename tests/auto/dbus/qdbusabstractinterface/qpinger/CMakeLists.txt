# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qpinger Binary:
#####################################################################

bobui_internal_add_executable(qpinger
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    NO_INSTALL
    SOURCES
        ../interface.cpp ../interface.h
        qpinger.cpp
    LIBRARIES
        BobUI::DBus
)
