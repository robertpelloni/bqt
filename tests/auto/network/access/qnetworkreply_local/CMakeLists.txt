# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
bobui_internal_add_test(tst_qnetworkreply_local
    SOURCES
        minihttpserver.h
        tst_qnetworkreply_local.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::NetworkPrivate
    BUNDLE_ANDROID_OPENSSL_LIBS
)
