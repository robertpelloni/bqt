# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

define_property(TARGET
    PROPERTY
        BOBUI_PLUGINS
    BRIEF_DOCS
        "List of BobUI plug-ins associated with a given BobUI module."
    FULL_DOCS
        "This is a property on BobUI modules.
        For instance, sqlite;odbc for Sql"
)

define_property(TARGET
    PROPERTY
        MODULE_PLUGIN_TYPES
    BRIEF_DOCS
        "List of plugin categories associated to the BobUI module"
    FULL_DOCS
        "This is a property on BobUI modules.
        For instance, sqldrivers for Sql."
)

define_property(TARGET
    PROPERTY
        BOBUI_PLUGIN_CLASS_NAME
    BRIEF_DOCS
        "Class name of the BobUI plug-in"
    FULL_DOCS
        "This is a property on BobUI plug-ins.
        For instance, QICOPlugin for the qico plug-in"
)

define_property(TARGET
    PROPERTY
        BOBUI_PLUGIN_TYPE
    BRIEF_DOCS
        "Type of the BobUI plug-in"
    FULL_DOCS
        "This is a property on BobUI plug-ins.
        For example, the value of the BOBUI_PLUGIN_TYPE property on the qico plugin is \"imageformats\""
)

define_property(TARGET
    PROPERTY
        BOBUI_MODULE
    BRIEF_DOCS
        "BobUI module associated with a plug-in."
    FULL_DOCS
        "This is a property on BobUI plug-ins.
        For instance, Sql for qsqlite"
)

define_property(TARGET
    PROPERTY
        BOBUI_DEFAULT_PLUGIN
    BRIEF_DOCS
        "Indicates whether a plug-in is added by default."
    FULL_DOCS
        "This is a property on BobUI plug-ins.
        It is mainly used to indicate if a plug-in should be added
        to the default set of plug-ins when building a static app -
        for instance, which QPA should be linked."
)

define_property(TARGET
    PROPERTY
        BOBUI_QML_MODULE_TARGET_PATH
    BRIEF_DOCS
        "Specifies the target path for a qml module"
    FULL_DOCS
        "Specifies the target path for a qml module"
)

define_property(TARGET
    PROPERTY
        BOBUI_QML_MODULE_URI
    BRIEF_DOCS
        "Specifies the URI for a qml module"
    FULL_DOCS
        "Specifies the URI for a qml module"
)

define_property(TARGET
    PROPERTY
        BOBUI_RESOURCE_PREFIX
    BRIEF_DOCS
        "Specifies the default BobUI resource prefix."
    FULL_DOCS
        "When using bobui_add_resource() without a PREFIX, then prefix of this target property
        will be used."
)

define_property(TARGET
    PROPERTY
        BOBUI_QML_MODULE_VERSION
    BRIEF_DOCS
        "Specifies the qml module's version."
    FULL_DOCS
        "Specifies the qml module's version."
)

define_property(GLOBAL
    PROPERTY
        BOBUI_TARGETS_FOLDER
    BRIEF_DOCS
        "Name of the FOLDER for targets internally created by AUTOGEN and BobUI's CMake API."
    FULL_DOCS
        "This property is used to initialize AUTOGEN_TARGETS_FOLDER and the FOLDER property of
        internal targets created by BobUI's CMake commands."
)
