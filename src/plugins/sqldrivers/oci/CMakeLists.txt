# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QOCIDriverPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QOCIDriverPlugin
    OUTPUT_NAME qsqloci
    PLUGIN_TYPE sqldrivers
    SOURCES
        main.cpp
        qsql_oci.cpp qsql_oci_p.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
    LIBRARIES
        Oracle::OCI
        BobUI::Core
        BobUI::CorePrivate
        BobUI::SqlPrivate
)

## Scopes:
#####################################################################

bobui_internal_force_macos_intel_arch(QOCIDriverPlugin)
