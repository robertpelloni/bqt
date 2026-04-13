# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_FEATURE_securetransport AND BOBUI_FEATURE_ssl)
    add_subdirectory(securetransport)
endif()

if ((BOBUI_FEATURE_openssl OR BOBUI_FEATURE_openssl_linked) AND BOBUI_FEATURE_ssl)
    add_subdirectory(openssl)
endif()

if (BOBUI_FEATURE_schannel AND BOBUI_FEATURE_ssl)
    add_subdirectory(schannel)
endif()

add_subdirectory(certonly)
