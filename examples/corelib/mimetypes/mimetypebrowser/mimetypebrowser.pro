TEMPLATE = app
BOBUI += widgets
requires(bobuiConfig(treeview))
CONFIG -= app_bundle
CONFIG += c++11

SOURCES += \
    main.cpp \
    mimetypemodel.cpp \
    mainwindow.cpp

HEADERS += \
    mimetypemodel.h \
    mainwindow.h

target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/mimetypes/mimetypebrowser
INSTALLS += target
