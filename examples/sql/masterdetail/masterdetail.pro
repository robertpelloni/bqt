HEADERS   = database.h \
            dialog.h \
            mainwindow.h
RESOURCES = masterdetail.qrc
SOURCES   = dialog.cpp \
            main.cpp \
            mainwindow.cpp

BOBUI += sql widgets
BOBUI += xml widgets
requires(bobuiConfig(tableview))

EXAMPLE_FILES = albumdetails.xml

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/sql/masterdetail
INSTALLS += target
