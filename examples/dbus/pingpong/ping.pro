BOBUI -= gui
BOBUI += dbus

HEADERS += ping-common.h
SOURCES += ping.cpp

target.path = $$[BOBUI_INSTALL_EXAMPLES]/dbus/pingpong
INSTALLS += target
