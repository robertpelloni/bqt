# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include(BobUIFindWrapHelper NO_POLICY_SCOPE)

bobui_find_package_system_or_bundled(wrap_pcre2
    FRIENDLY_PACKAGE_NAME "PCRE2"
    WRAP_PACKAGE_TARGET "WrapPCRE2::WrapPCRE2"
    WRAP_PACKAGE_FOUND_VAR_NAME "WrapPCRE2_FOUND"
    BUNDLED_PACKAGE_NAME "BundledPcre2"
    BUNDLED_PACKAGE_TARGET "BundledPcre2"
    SYSTEM_PACKAGE_NAME "WrapSystemPCRE2"
    SYSTEM_PACKAGE_TARGET "WrapSystemPCRE2::WrapSystemPCRE2"
)
