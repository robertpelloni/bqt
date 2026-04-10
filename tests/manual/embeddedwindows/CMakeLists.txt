# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(embeddedwindows
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
    GUI
)

bobui_internal_extend_target(embeddedwindows CONDITION WIN32
    SOURCES
        app.manifest
)

if(BOBUI_FEATURE_xcb)
    target_link_libraries(embeddedwindows PRIVATE XCB::XCB)
endif()

if(APPLE)
    enable_language(OBJCXX)
    set_source_files_properties(main.cpp PROPERTIES LANGUAGE OBJCXX)
endif()
