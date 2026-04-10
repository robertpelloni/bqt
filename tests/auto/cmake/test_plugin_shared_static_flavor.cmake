# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

_bobui_internal_test_expect_pass(test_plugin_shared_static_flavor
                              TESTNAME test_plugin_flavor_static
                              BUILD_OPTIONS
                              "-DPLUGIN_OPTIONS=STATIC"
                              "-DEXPECTED_PLUGIN_TARGET_TYPE=STATIC_LIBRARY")

_bobui_internal_test_expect_pass(test_plugin_shared_static_flavor
                              TESTNAME test_plugin_flavor_shared
                              BUILD_OPTIONS
                              "-DPLUGIN_OPTIONS=SHARED"
                              "-DEXPECTED_PLUGIN_TARGET_TYPE=MODULE_LIBRARY")

if(BOBUI6_IS_SHARED_LIBS_BUILD)
    set(expected_plugin_target_type "MODULE_LIBRARY")
else()
    set(expected_plugin_target_type "STATIC_LIBRARY")
endif()

# Check default computed value when no explicit option is set.
_bobui_internal_test_expect_pass(test_plugin_shared_static_flavor
                              TESTNAME test_plugin_flavor_derived_from_bobui_type
                              BUILD_OPTIONS
                              "-DPLUGIN_OPTIONS="
                              "-DEXPECTED_PLUGIN_TARGET_TYPE=${expected_plugin_target_type}")
