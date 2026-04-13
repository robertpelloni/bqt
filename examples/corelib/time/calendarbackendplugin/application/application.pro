TEMPLATE = app
TARGET = application
INCLUDEPATH += . \
    ../common/
BOBUI += core core-private widgets

target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/datetime/calendarbackendplugin/application
INSTALLS += target

SOURCES += main.cpp
HEADERS += ../common/calendarBackendInterface.h
