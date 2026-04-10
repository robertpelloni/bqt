BOBUI       += core gui

greaterThan(BOBUI_MAJOR_VERSION, 4): BOBUI += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += BOBUI_DISABLE_DEPRECATED_UP_TO=0x060000 # disables all APIs deprecated in BobUI 6.0.0 and earlier

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    data.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
