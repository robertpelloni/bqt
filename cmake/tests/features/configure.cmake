# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#### Features

# This belongs into gui, but the license check needs it here already.
bobui_feature("top_a" PRIVATE
    LABEL "top_a"
    CONDITION ON
)
bobui_feature("top_b" PUBLIC PRIVATE
    LABEL "top_b"
    AUTODETECT OFF
)
bobui_feature_definition("top_a" "top_defa")

bobui_feature("top_enabled" PRIVATE
    LABEL "top_enabled"
    ENABLE ON
)

bobui_feature("top_disabled" PRIVATE
    LABEL "top_enabled"
    DISABLE ON
)

bobui_feature("top_disabled_enabled" PRIVATE
    LABEL "top_enabled_enabled"
    DISABLE ON
    ENABLE ON
)

bobui_feature("top_not_emitted" PRIVATE
    LABEL "top_not_emitted"
    EMIT_IF OFF
)

bobui_extra_definition("top_extra" "PUBLIC_FOO" PUBLIC)
