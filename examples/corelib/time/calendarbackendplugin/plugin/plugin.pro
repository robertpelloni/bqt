TEMPLATE = lib
TARGET = calendarPlugin
INCLUDEPATH += . \
    ../common/
BOBUI += core core-private widgets

HEADERS += calendarbackend.h calendarplugin.h
SOURCES += calendarbackend.cpp calendarplugin.cpp

target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/datetime/calendarbackendplugin/plugin
INSTALLS += target
