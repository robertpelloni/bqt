TEMPLATE = app
TARGET = fontfeatures
INCLUDEPATH += .
BOBUI += widgets

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of BobUI.
#DEFINES += BOBUI_DISABLE_DEPRECATED_UP_TO=0x060000 # disables all APIs deprecated in BobUI 6.0.0 and earlier

# Input
HEADERS += mainwindow.h
FORMS += mainwindow.ui
SOURCES += main.cpp \
           mainwindow.cpp \
