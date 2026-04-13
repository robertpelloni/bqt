# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_FEATURE_desktopservices)
    add_subdirectory(qdesktopservices)
endif()
add_subdirectory(qdoublevalidator)
add_subdirectory(qintvalidator)
add_subdirectory(qregularexpressionvalidator)
add_subdirectory(bobuiexturefilereader)
if(BOBUI_FEATURE_undogroup)
    add_subdirectory(qundogroup)
endif()
if(BOBUI_FEATURE_undocommand)
    add_subdirectory(qundostack)
endif()
