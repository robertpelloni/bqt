TEMPLATE = app
INCLUDEPATH += .

HEADERS     = bookdelegate.h bookwindow.h initdb.h
RESOURCES   = books.qrc
SOURCES     = bookdelegate.cpp main.cpp bookwindow.cpp

BOBUI += sql widgets widgets
requires(bobuiConfig(tableview))

target.path = $$[BOBUI_INSTALL_EXAMPLES]/sql/books
INSTALLS += target
