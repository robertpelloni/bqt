BOBUI *= testlib

SOURCES += \
        $$PWD/qbaselinetest.cpp

HEADERS += \
        $$PWD/qbaselinetest.h

bobuiHaveModule(widgets) {
        SOURCES += $$PWD/qwidgetbaselinetest.cpp
        HEADERS += $$PWD/qwidgetbaselinetest.h
}

include($$PWD/baselineprotocol.pri)
