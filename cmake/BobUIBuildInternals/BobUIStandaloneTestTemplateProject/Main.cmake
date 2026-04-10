# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Includes BobUISetup and friends for private CMake API.
set(BOBUI_INTERNAL_IS_STANDALONE_TEST TRUE)

# Make find_package(BobUI6 COMPONENTS Foo) pull in FooPrivate too.
set(BOBUI_FIND_PRIVATE_MODULES TRUE)

# Checks minimum CMake version and upgrades policies.
bobui_internal_project_setup()

# Look for the BobUI6 package before trying to call bobui_build_internals_set_up_private_api,
# otherwise it will fail when using a cross-compiled BobUI, because BOBUI_HOST_PATH will not be set.
# BOBUI_HOST_PATH is set by BobUI6Dependencies.cmake.
find_package(BobUI6 REQUIRED)

# Includes BobUISetup.cmake.
bobui_build_internals_set_up_private_api()

# Find all StandaloneTestsConfig.cmake files, and include them
# This will find all BobUI packages that are required for standalone tests.
# It will find more packages that needed for a certain test, but will ensure any test can
# be built.
bobui_get_standalone_parts_config_files_path(standalone_parts_config_path)

file(GLOB config_files "${standalone_parts_config_path}/*")
foreach(file ${config_files})
    include("${file}")
endforeach()

# Set language standards after finding Core, because that's when the relevant
# feature variables are available.
bobui_set_language_standards()

# Just before adding the test, change the local (non-cache) install prefix to something other than
# the BobUI install prefix, so that tests don't try to install and pollute the BobUI install prefix.
# Needs to be called after bobui_get_standalone_parts_config_files_path().
bobui_internal_set_up_fake_standalone_parts_install_prefix()
