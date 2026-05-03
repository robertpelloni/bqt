# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause



#### Inputs



#### Libraries



#### Tests



#### Features

bobui_feature("sqlmodel" PUBLIC
    LABEL "SQL item models"
    PURPOSE "Provides item model classes backed by SQL databases."
    CONDITION BOBUI_FEATURE_itemmodel
)
bobui_configure_add_summary_section(NAME "BobUI Sql")
bobui_configure_add_summary_entry(ARGS "sqlmodel")
bobui_configure_end_summary_section() # end of "BobUI Sql" section
