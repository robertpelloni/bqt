INCLUDEPATH += $$PWD
SOURCES += \
    $$PWD/textdump.cpp \
    $$PWD/eventfilter.cpp \
    $$PWD/qwindowdump.cpp

HEADERS += \
    $$PWD/textdump.h \
    $$PWD/eventfilter.h \
    $$PWD/qwindowdump.h \
    $$PWD/nativewindowdump.h

win32:  {
    SOURCES += $$PWD/nativewindowdump_win.cpp
    LIBS += -luser32
} else {
    SOURCES += $$PWD/nativewindowdump.cpp
}

BOBUI += gui-private core-private
contains(BOBUI, widgets) {
    HEADERS += \
        $$PWD/qwidgetdump.h \
        $$PWD/debugproxystyle.h \
        $$PWD/logwidget.h

    SOURCES += \
        $$PWD/qwidgetdump.cpp \
        $$PWD/debugproxystyle.cpp \
        $$PWD/logwidget.cpp
    BOBUI += widgets-private
}

contains(BOBUI, opengl) {
    HEADERS += \
        $$PWD/glinfo.h

    SOURCES += \
        $$PWD/glinfo.cpp

    BOBUI += opengl openglwidgets
}

DEFINES += BOBUI_DIAG_LIB
