# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

add_library(testlib_code_snippets OBJECT
    doc_src_bobuiqskip.cpp
    src_corelib_kernel_bobuiestsupport_core.cpp
)

target_link_libraries(testlib_code_snippets PRIVATE
    BobUI::Core
    BobUI::Test
)

if(BOBUI_FEATURE_gui AND BOBUI_FEATURE_sql)
    target_sources(testlib_code_snippets PRIVATE
        doc_src_bobuiestlib.cpp
    )
    target_link_libraries(testlib_code_snippets PUBLIC
        BobUI::Gui
        BobUI::Sql
    )
endif()

if(BOBUI_FEATURE_widgets)
    target_sources(testlib_code_snippets PRIVATE
        doc_src_bobuiestevent.cpp
        doc_src_bobuiestevent.h
    )
    target_link_libraries(testlib_code_snippets PUBLIC
        BobUI::Widgets
    )
endif()
