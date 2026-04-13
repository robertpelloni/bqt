# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_quuid Test:
#####################################################################

bobui_internal_add_test(tst_quuid
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../tst_quuid.cpp
    LIBRARIES
        BobUI::TestPrivate
)

## Scopes:

bobui_internal_extend_target(tst_quuid CONDITION APPLE
    SOURCES
        ../tst_quuid_darwin.mm
    LIBRARIES
        BobUI::CorePrivate
        ${FWFoundation}
)

if(BOBUI_FEATURE_process AND NOT ANDROID)
    add_dependencies(tst_quuid testProcessUniqueness)
endif()
