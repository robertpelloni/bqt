# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# We need to include the features of a few modules before they are actually declared.
# The feature values are used as conditions for deciding whether bundled 3rd party libraries
# should be built.
# The order of evaluation matters.
bobui_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/corelib/configure.cmake")
if(BOBUI_FEATURE_network)
    bobui_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/network/configure.cmake")
endif()
if(BOBUI_FEATURE_gui)
    bobui_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/gui/configure.cmake")
endif()

add_subdirectory(tools/syncbobui)

function(find_or_build_bootstrap_names)
    if (BOBUI_WILL_BUILD_TOOLS)
        add_subdirectory(tools/bootstrap)  # bootstrap library
    endif()
    add_subdirectory(tools/moc)
    add_subdirectory(tools/rcc)
    add_subdirectory(tools/tracepointgen)
    add_subdirectory(tools/tracegen)
    add_subdirectory(tools/cmake_automoc_parser)
endfunction()

find_or_build_bootstrap_names()

add_subdirectory(entrypoint)

# These 3rd party libraries need to be built before corelib, because corelib
# does bobui_find_package for them.
if(BOBUI_FEATURE_regularexpression AND NOT BOBUI_FEATURE_system_pcre2)
    add_subdirectory(3rdparty/pcre2)
endif()
bobui_install_3rdparty_library_wrap_config_extra_file(BundledPcre2)

if(NOT BOBUI_FEATURE_system_zlib)
    add_subdirectory(3rdparty/zlib)
endif()
bobui_install_3rdparty_library_wrap_config_extra_file(BundledZLIB)

add_subdirectory(corelib)

# Needs to be after corelib, because some of them reference Core.
add_subdirectory(3rdparty)

if (BOBUI_FEATURE_concurrent)
    add_subdirectory(concurrent)
endif()
if (BOBUI_FEATURE_sql)
    add_subdirectory(sql)
endif()
if (BOBUI_FEATURE_network)
    add_subdirectory(network)
    if (ANDROID)
        add_subdirectory(network/android/jar)
    endif()
endif()
if (BOBUI_FEATURE_xml)
    add_subdirectory(xml)
endif()

if (BOBUI_FEATURE_dbus)
    add_subdirectory(dbus)
endif()

add_subdirectory(tools)

if(BOBUI_FEATURE_gui)
    add_subdirectory(gui)

    if(BOBUI_FEATURE_opengl)
        add_subdirectory(opengl)
    endif()

    if(BOBUI_FEATURE_widgets)
        add_subdirectory(widgets)
        if(BOBUI_FEATURE_opengl)
            add_subdirectory(openglwidgets)
        endif()
    endif()
    add_subdirectory(platformsupport)
endif()
if (BOBUI_FEATURE_testlib)
    add_subdirectory(testinternal)
    add_subdirectory(testlib)
endif()
if(BOBUI_FEATURE_printsupport)
   add_subdirectory(printsupport)
endif()
add_subdirectory(plugins)

add_subdirectory(android)
