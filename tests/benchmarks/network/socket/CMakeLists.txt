# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_FEATURE_localserver)
    add_subdirectory(qlocalsocket)
endif()

if(BOBUI_FEATURE_udpsocket)
    add_subdirectory(qudpsocket)
endif()

add_subdirectory(bobuicpserver)
