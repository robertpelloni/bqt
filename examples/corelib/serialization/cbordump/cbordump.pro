BOBUI += core
BOBUI -= gui

TARGET = cbordump
CONFIG += cmdline

TEMPLATE = app

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/serialization/cbordump
INSTALLS += target

SOURCES += main.cpp
