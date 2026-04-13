# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## testapp Binary:
#####################################################################

bobui_internal_add_executable(windeploy_testapp
    GUI
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
)

set_target_properties(windeploy_testapp
    PROPERTIES
        OUTPUT_NAME testapp
)
