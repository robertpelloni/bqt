# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## embeddedintoforeignwindow Binary:
#####################################################################

bobui_internal_add_manual_test(embeddedintoforeignwindow
    SOURCES
        ../diaglib/eventfilter.cpp ../diaglib/eventfilter.h
        ../diaglib/nativewindowdump.h
        ../diaglib/qwindowdump.cpp ../diaglib/qwindowdump.h
        ../diaglib/textdump.cpp ../diaglib/textdump.h
        itemwindow.cpp itemwindow.h
        main.cpp
    NO_PCH_SOURCES
        itemwindow.cpp # undef BOBUI_NO_FOREACH
    DEFINES
        BOBUI_DIAG_LIB
    INCLUDE_DIRECTORIES
        ../diaglib
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(embeddedintoforeignwindow CONDITION WIN32
    SOURCES
        ../diaglib/nativewindowdump_win.cpp
    LIBRARIES
        user32
)

bobui_internal_extend_target(embeddedintoforeignwindow CONDITION UNIX
    SOURCES
        ../diaglib/nativewindowdump.cpp
)

bobui_internal_extend_target(embeddedintoforeignwindow CONDITION BOBUI_FEATURE_widgets
    SOURCES
        ../diaglib/debugproxystyle.cpp ../diaglib/debugproxystyle.h
        ../diaglib/logwidget.cpp ../diaglib/logwidget.h
        ../diaglib/qwidgetdump.cpp ../diaglib/qwidgetdump.h
    LIBRARIES
        BobUI::WidgetsPrivate
)

bobui_internal_extend_target(embeddedintoforeignwindow CONDITION BOBUI_FEATURE_opengl
    SOURCES
        ../diaglib/glinfo.cpp ../diaglib/glinfo.h
    LIBRARIES
        BobUI::OpenGL
        BobUI::OpenGLWidgets
)
