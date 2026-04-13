BOBUI -= gui
BOBUI += dbus

HEADERS += complexping.h ping-common.h
SOURCES += complexping.cpp

target.path = $$[BOBUI_INSTALL_EXAMPLES]/dbus/complexpingpong
INSTALLS += target
