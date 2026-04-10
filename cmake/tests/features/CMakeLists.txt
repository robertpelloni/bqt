# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(FeaturesTest
    VERSION 1.0.0
    DESCRIPTION "BobUIFeature test"
    HOMEPAGE_URL "https://bobui.io/"
    LANGUAGES CXX C
)

## Add some paths to check for cmake modules:
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../;${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/extra-cmake-modules/find-modules;${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/kwin")

## BobUI specific setup common for all modules:
include(BobUISetup)

## Library to hold global features:
add_library(GlobalConfig INTERFACE)

bobui_feature_module_begin(LIBRARY GlobalConfig
    PUBLIC_FILE src/corelib/global/qconfig.h
    PRIVATE_FILE src/corelib/global/qconfig_p.h
)
include("${CMAKE_CURRENT_SOURCE_DIR}/configure.cmake")
bobui_feature_module_end(GlobalConfig)

assert(BOBUI_FEATURE_top_a STREQUAL "ON")
assert(BOBUI_FEATURE_top_b STREQUAL "OFF")
assert(BOBUI_FEATURE_top_enabled STREQUAL "ON")
assert(BOBUI_FEATURE_top_disabled STREQUAL "OFF")
assert(BOBUI_FEATURE_top_disabled_enabled STREQUAL "OFF")
assert(BOBUI_FEATURE_top_not_emitted STREQUAL "OFF")

## Enable feature summary at the end of the configure run:
include(FeatureSummary)

add_subdirectory(src)

## Delayed actions on some of the BobUI targets:
include(BobUIPostProcess)

## Print a feature summary:
feature_summary(WHAT PACKAGES_FOUND PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)
