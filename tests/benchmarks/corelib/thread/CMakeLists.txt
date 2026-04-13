# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_FEATURE_future)
    add_subdirectory(qfuture)
endif()
add_subdirectory(qmutex)
add_subdirectory(qreadwritelock)
add_subdirectory(bobuihreadstorage)
add_subdirectory(bobuihreadpool)
add_subdirectory(qwaitcondition)
