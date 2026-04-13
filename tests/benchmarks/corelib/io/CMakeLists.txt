# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(qdir)
add_subdirectory(qdiriterator)
add_subdirectory(qfile)
add_subdirectory(qfileinfo)
add_subdirectory(qiodevice)
if(BOBUI_FEATURE_process)
    add_subdirectory(qprocess)
endif()
add_subdirectory(bobuiemporaryfile)
add_subdirectory(bobuiextstream)
add_subdirectory(qurl)
