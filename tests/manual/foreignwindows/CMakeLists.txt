# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## foreignwindows Binary:
#####################################################################

bobui_internal_add_manual_test(foreignwindows
    SOURCES
        ../diaglib/eventfilter.cpp ../diaglib/eventfilter.h
        ../diaglib/nativewindowdump.h
        ../diaglib/qwindowdump.cpp ../diaglib/qwindowdump.h
        ../diaglib/textdump.cpp ../diaglib/textdump.h
        main.cpp
    DEFINES
        BOBUI_DIAG_LIB
    INCLUDE_DIRECTORIES
        ../diaglib
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::OpenGL
        BobUI::Widgets
)

## Scopes:
#####################################################################

bobui_internal_extend_target(foreignwindows CONDITION WIN32
    SOURCES
        ../diaglib/nativewindowdump_win.cpp
    LIBRARIES
        user32
)

bobui_internal_extend_target(foreignwindows CONDITION UNIX
    SOURCES
        ../diaglib/nativewindowdump.cpp
)

bobui_internal_extend_target(foreignwindows CONDITION BOBUI_FEATURE_widgets
    SOURCES
        ../diaglib/debugproxystyle.cpp ../diaglib/debugproxystyle.h
        ../diaglib/logwidget.cpp ../diaglib/logwidget.h
        ../diaglib/qwidgetdump.cpp ../diaglib/qwidgetdump.h
    LIBRARIES
        BobUI::WidgetsPrivate
)

bobui_internal_extend_target(foreignwindows CONDITION BOBUI_FEATURE_opengl
    SOURCES
        ../diaglib/glinfo.cpp ../diaglib/glinfo.h
    LIBRARIES
        BobUI::OpenGL
        BobUI::OpenGLWidgets
)
