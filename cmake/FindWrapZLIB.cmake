# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include(BobUIFindWrapHelper NO_POLICY_SCOPE)

bobui_find_package_system_or_bundled(wrap_zlib
    FRIENDLY_PACKAGE_NAME "ZLIB"
    WRAP_PACKAGE_TARGET "WrapZLIB::WrapZLIB"
    WRAP_PACKAGE_FOUND_VAR_NAME "WrapZLIB_FOUND"
    BUNDLED_PACKAGE_NAME "BundledZLIB"
    BUNDLED_PACKAGE_TARGET "BundledZLIB"
    SYSTEM_PACKAGE_NAME "WrapSystemZLIB"
    SYSTEM_PACKAGE_TARGET "WrapSystemZLIB::WrapSystemZLIB"
)
