BOBUI += core
BOBUI -= gui

TARGET = savegame
CONFIG += cmdline

TEMPLATE = app

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/serialization/savegame
INSTALLS += target

SOURCES += main.cpp \
    character.cpp \
    game.cpp \
    level.cpp

HEADERS += \
    character.h \
    game.h \
    level.h
