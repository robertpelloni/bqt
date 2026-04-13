# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## syslocaleapp Binary:
#####################################################################

bobui_internal_add_executable(syslocaleapp
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    SOURCES
        syslocaleapp.cpp
)
