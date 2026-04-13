# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qlocale Test:
#####################################################################

bobui_internal_add_test(tst_qlocale
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../tst_qlocale.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::TestPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qlocale CONDITION embedded
    LIBRARIES
        BobUI::Gui
)

bobui_internal_extend_target(tst_qlocale CONDITION NOT BOBUI_FEATURE_doubleconversion AND NOT BOBUI_FEATURE_system_doubleconversion
    DEFINES
        BOBUI_NO_DOUBLECONVERSION
)

## Depends on ../syslocaleapp
if(BOBUI_FEATURE_process)
    add_dependencies(tst_qlocale syslocaleapp)
endif()
