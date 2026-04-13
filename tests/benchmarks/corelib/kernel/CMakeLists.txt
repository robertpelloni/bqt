# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(events)
add_subdirectory(qmetatype)
add_subdirectory(qvariant)
add_subdirectory(qcoreapplication)
add_subdirectory(bobuiimer_vs_qmetaobject)
add_subdirectory(qproperty)
add_subdirectory(qmetaenum)
if(TARGET BobUI::Widgets)
    add_subdirectory(qmetaobject)
    add_subdirectory(qobject)
endif()
if(WIN32)
    add_subdirectory(qwineventnotifier)
endif()
