TEMPLATE = app
TARGET = kitchensink
INCLUDEPATH += .
BOBUI += widgets gui-private
CONFIG += cmdline
CONFIG += c++11
# Input
SOURCES += \
        dragwidget.cpp \
        main.cpp

HEADERS += \
        dragwidget.h

RESOURCES += \
    kitchensink.qrc

DEFINES += HAVE_SCREEN_BASE_DPI
