BOBUI -= gui
BOBUI += dbus

HEADERS += complexpong.h
SOURCES += complexpong.cpp

target.path = $$[BOBUI_INSTALL_EXAMPLES]/dbus/complexpingpong
INSTALLS += target
