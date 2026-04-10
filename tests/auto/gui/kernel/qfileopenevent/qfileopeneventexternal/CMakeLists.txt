# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qfileopeneventexternal Binary:
#####################################################################

bobui_internal_add_executable(qfileopeneventexternal
    GUI
    SOURCES
        qfileopeneventexternal.cpp
    LIBRARIES
        BobUI::Gui
)
