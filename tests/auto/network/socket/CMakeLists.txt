# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_FEATURE_private_tests)
    add_subdirectory(qhttpsocketengine)
    add_subdirectory(bobuicpsocket)
    add_subdirectory(qsocks5socketengine)
    add_subdirectory(platformsocketengine)
endif()
if(BOBUI_FEATURE_udpsocket)
    add_subdirectory(qudpsocket)
endif()
add_subdirectory(qabstractsocket)
if(BOBUI_FEATURE_localserver AND NOT ANDROID)
    # BOBUIBUG-87387
    add_subdirectory(qlocalsocket)
endif()

if(BOBUI_FEATURE_networkinterface AND NOT ANDROID)
    # BOBUIBUG-87388
    add_subdirectory(bobuicpserver)
endif()

if(BOBUI_FEATURE_sctp)
    add_subdirectory(qsctpsocket)
endif()
