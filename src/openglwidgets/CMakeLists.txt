# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## OpenGLWidgets Module:
#####################################################################

bobui_internal_add_module(OpenGLWidgets
    NO_PRIVATE_MODULE
    SOURCES
        qopenglwidget.cpp qopenglwidget.h
        bobuiopenglwidgetsglobal.h
    DEFINES
        BOBUI_NO_CONTEXTLESS_CONNECT
        BOBUI_NO_FOREACH
        BOBUI_NO_URL_CAST_FROM_STRING
        BOBUI_NO_USING_NAMESPACE
        BOBUI_USE_NODISCARD_FILE_OPEN
    LIBRARIES
        BobUI::OpenGLPrivate
        BobUI::WidgetsPrivate
    PUBLIC_LIBRARIES
        BobUI::OpenGL
        BobUI::Widgets
)
