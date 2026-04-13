# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_BUILD_STANDALONE_TESTS)
    # Add bobui_find_package calls for extra dependencies that need to be found when building
    # the standalone tests here.

    # Needed for early feature values, despite it being found later on in bobui_build_tests().
    # Needs to be find_package, not bobui_find_package, because bobui_find_package doesn't handle finding
    # component for a super-package that has already been found.
    find_package(BobUI6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS Core)

    # Modify the optimization flags specifically for bobuibase standalone tests. Other projects
    # are handled by bobui_enable_cmake_languages().
    bobui_internal_set_up_config_optimizations_like_in_qmake()

    bobui_find_package(WrapDBus1 MODULE PROVIDED_TARGETS dbus-1)
    bobui_find_package(ICU 50.1 COMPONENTS i18n uc data PROVIDED_TARGETS ICU::i18n ICU::uc ICU::data)
    bobui_find_package(WrapOpenSSL MODULE PROVIDED_TARGETS WrapOpenSSL::WrapOpenSSL)
    bobui_find_package(WrapOpenSSLHeaders MODULE PROVIDED_TARGETS WrapOpenSSLHeaders::WrapOpenSSLHeaders)
endif()
bobui_build_tests()
if(FEATURE_squish_test)
    add_subdirectory(squish)
endif()
