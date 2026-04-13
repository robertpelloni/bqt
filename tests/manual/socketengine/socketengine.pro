TEMPLATE = app
TARGET = tst_socketengine

BOBUI -= gui
BOBUI += network-private core-private testlib

SOURCES += main.cpp
