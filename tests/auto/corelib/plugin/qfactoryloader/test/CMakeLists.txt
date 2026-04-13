# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qfactoryloader Test:
#####################################################################

bobui_internal_add_test(tst_qfactoryloader
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../plugin1/plugininterface1.h
        ../plugin2/plugininterface2.h
        ../tst_qfactoryloader.cpp
    LIBRARIES
        BobUI::CorePrivate
        tst_qfactoryloader_staticplugin
)

bobui_internal_extend_target(tst_qfactoryloader CONDITION NOT BOBUI_FEATURE_library
    LIBRARIES
        tst_qfactoryloader_plugin1
        tst_qfactoryloader_plugin2
)

add_dependencies(tst_qfactoryloader tst_qfactoryloader_plugin1 tst_qfactoryloader_plugin2)

if(ANDROID)
    # BOBUI_ANDROID_EXTRA_PLUGINS requires a list of directories, not files!
    set_target_properties(tst_qfactoryloader PROPERTIES
        BOBUI_ANDROID_EXTRA_PLUGINS "${CMAKE_CURRENT_BINARY_DIR}/../bin"
    )
endif()
