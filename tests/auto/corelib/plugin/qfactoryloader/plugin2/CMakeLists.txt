# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## plugin2 Generic Library:
#####################################################################

bobui_internal_add_cmake_library(tst_qfactoryloader_plugin2
    MODULE
    INSTALL_DIRECTORY "${INSTALL_TESTSDIR}/tst_qfactoryloader/bin"
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../bin"
    SOURCES
        plugin2.cpp plugin2.h
    LIBRARIES
        BobUI::Core
)

if(ANDROID)
    # On Android the plugins must match the following mask:
    # "libplugins_{suffix}_*.so"
    # and the testcase uses "bin" as a suffix
    set_target_properties(tst_qfactoryloader_plugin2 PROPERTIES
        OUTPUT_NAME "plugins_bin_tst_qfactoryloader_plugin2")
endif()

bobui_internal_extend_target(tst_qfactoryloader_plugin2 CONDITION NOT BOBUI_FEATURE_library
    DEFINES
        BOBUI_STATICPLUGIN
)

bobui_autogen_tools_initial_setup(tst_qfactoryloader_plugin2)
