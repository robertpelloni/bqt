# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

if(NOT TARGET BobUI6::Widgets)
    return()
endif()
bobui_internal_add_example(books)
bobui_internal_add_example(drilldown)
bobui_internal_add_example(cachedtable)
bobui_internal_add_example(querymodel)
bobui_internal_add_example(relationaltablemodel)
bobui_internal_add_example(sqlwidgetmapper)
bobui_internal_add_example(tablemodel)
if(TARGET BobUI6::Xml)
    bobui_internal_add_example(masterdetail)
endif()

if(NOT CMAKE_CROSSCOMPILING)
    bobui_internal_add_example(sqlbrowser)
endif()
