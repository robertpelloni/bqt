# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## copier Binary:
#####################################################################

set(args OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

bobui_internal_add_executable(copier
    ${args}
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
)
