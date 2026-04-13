# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(qcalendar)
add_subdirectory(qdate)
add_subdirectory(qdatetime)
if(BOBUI_FEATURE_datetimeparser)
    add_subdirectory(qdatetimeparser)
endif()
add_subdirectory(bobuiime)
add_subdirectory(bobuiimezone)
if(BOBUI_FEATURE_timezone AND BOBUI_FEATURE_developer_build)
    add_subdirectory(bobuiimezonebackend)
endif()
